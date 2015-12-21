#-------------------------------------------------------------------------------

_       = require 'underscore'
colours = require 'colors'
path    = require 'path'


#-------------------------------------------------------------------------------
# this is wehere we do any pre-processing required on the json data returned
# from reflector.
#-------------------------------------------------------------------------------

module.exports = ( file_in ) ->

  #-------------------------------------------------------------------------------
  # load json file

  console.log "Processing", "#{ file_in }".cyan
  data = require file_in


  #-------------------------------------------------------------------------------
  # validate data

  _.each data, (current) ->

    return unless current.inherits?

    for inhertied in current.inherits

      parent = _.find data, (decl) -> decl.name == inhertied.type
      console.log "[WARNING]".magenta, "failed to find inherited class", inhertied.type.yellow if not parent?
      console.log "[WARNING]".magenta, "virtual inheritance not supported" if inhertied.virtual? and inhertied.virtual == "true"


  #-------------------------------------------------------------------------------
  # extract header files we may need to include later

  includes = Object.keys _.groupBy data, (decl) -> path.basename( decl.location ).replace /:\d+:\d+/, ""
  includes = _.filter includes, (filename) -> filename.toLowerCase().indexOf( ".h" ) > 0



  #-------------------------------------------------------------------------------
  # remove private fields (we don't want to reflect these)

  _.each data, (current) ->
    return unless current.fields?
    current.fields = _.filter current.fields, (f) -> f.access != "private"


  #-------------------------------------------------------------------------------
  # array and pointer field types

  _.each data, (current) ->

    return unless current.fields?

    for f in current.fields

      if f.type.indexOf( "std::vector" ) == 0

        s = f.type.indexOf( "<" ) + 1
        e = f.type.indexOf ">"

        type = f.type.substr( s, e - s )
        p = type.indexOf " *"

        if p > 0
          f.type      = type.substr 0, p
          f.iterator  = "vector*"
        else
          f.type      = type
          f.iterator  = "vector"

        f.type = "this" if f.type == current.name

      else if f.type.indexOf( "[" ) >= 0

        console.log "[WARNING]".magenta, "array types not supported .. should write an iterator here! :)"

      else if f.type.indexOf( " *" ) > 0

        f.type    = f.type.substr 0, f.type.indexOf( " *" )
        f.pointer = true


  #-------------------------------------------------------------------------------
  # hoover up miscellaneous types

  misc = [
    "bool",
    "char",
    "unsigned char",
    "int",
    "unsigned int",
    "float",
    "double",
  ]

  _.each data, (current) ->

    return unless current.fields?

    for field in current.fields

      # if not standard type, seen already or in the json then add to list of "misc"

      continue if field.type == "this"
      continue if _.contains misc, field.type
      continue if _.find data, (f) -> f.name == field.type

      misc.push field.type

  #-------------------------------------------------------------------------------
  # merge base class fields
  #
  # NB: as we do these in order we don't need to recurse here
  #

  _.each data, (current) ->

    return unless current.inherits?

    # for each inherited class

    for inhertied in current.inherits

      # ignore private base classes

      continue if inhertied.access == "private"

      # find declaration in data

      parent = _.find data, (decl) -> decl.name == inhertied.type
      continue if not parent?


      # add parent fields to list for this class

      fields = []
      fields.push _.extend { from: parent.name }, field for field in parent.fields

      current.fields = [] unless current.fields?
      current.fields = fields.concat current.fields



  #-------------------------------------------------------------------------------
  # merge base classes
  #
  # do this after merging fields otherwise we'll copy the fields multiple times!
  #

  _.each data, (current) ->

    return unless current.inherits?

    bases = []

    for inhertied in current.inherits

      # get parent base classes

      parent = _.find data, (decl) -> decl.name == inhertied.type
      continue unless parent? and parent.inherits?

      for parent_base in parent.inherits

        # add to list of inherited classes if we haven't already

        continue if _.find bases, (i) -> i.type == parent_base.type
        continue if _.find current.inherits, (i) -> i.type == parent_base.type

        bases.push parent_base

    # join parent classes to ours

    current.inherits = current.inherits.concat bases



  #-------------------------------------------------------------------------------
  # return data for template

  return {
    includes  : includes
    misc      : misc
    classes   : data
  }

