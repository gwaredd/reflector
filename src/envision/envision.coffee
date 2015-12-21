#-------------------------------------------------------------------------------

fs      = require 'fs'
path    = require 'path'
colours = require 'colors'
dot     = require 'dot'
envis   = require './envisaged'

dot.templateSettings.strip = false


#-------------------------------------------------------------------------------

usage = ->

  coffee  = process.argv[ 0 ]
  script  = path.basename process.argv[ 1 ]

  console.log "Usage:".cyan, "#{ coffee } #{ script }", "<json file> <output cpp>".yellow
  process.exit()


#-------------------------------------------------------------------------------

try

  # read command line args

  usage() unless process.argv.length > 3

  file_in   = process.argv[ 2 ]
  file_out  = process.argv[ 3 ]


  # pre-process data and execute rtti.dot template

  output = dot.process( path: "." ).rtti
    file    : path.basename file_in
    date    : (new Date()).toString()
    data    : envis file_in

  # write out result

  fs.writeFileSync file_out, output

catch e

  console.log "[ERROR]".red, e

