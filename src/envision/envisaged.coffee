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
  # TODO: process field types

  # reflector returns the declared type as shown in the source, depending on our types
  # we may want to convert them here to something more RTTI friendly (sufficient for protoype though)
  #
  # e.g. char[32] -> char*
  #


  #-------------------------------------------------------------------------------
  # hoover up miscellaneous types

  misc = [
    "bool",
    "char",
    "unsigned char",
    # "short",
    # "unsigned short",
    "int",
    "unsigned int",
    # "long",
    # "unsigned long",
    "float",
    "double",
  ]

  _.each data, (current) ->

    return unless current.fields?

    for field in current.fields

      # if not standard type, seen already or in the json then add to list of "misc"

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

