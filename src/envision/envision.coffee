#-------------------------------------------------------------------------------

fs      = require 'fs'
path    = require 'path'
colours = require 'colors'
dot     = require 'dot'


# config

colours.setTheme
  warn : 'magenta'
  error: 'red'

dot.templateSettings.strip = false


#-------------------------------------------------------------------------------

usage = ->

  coffee  = process.argv[ 0 ]
  script  = path.basename process.argv[ 1 ]

  console.log "Usage:".cyan, "#{ coffee } #{ script }", "<json file>".yellow
  process.exit()


#-------------------------------------------------------------------------------

processJson = (file) ->

  console.log ""
  console.log "Processing '#{ file }'"

  return require file


#-------------------------------------------------------------------------------

try

  usage() unless process.argv.length > 2

  # load json and templates

  file    = process.argv[ 2 ]
  json    = processJson file
  tmpl    = dot.process path: "."

  # run templates with json

  output  = tmpl.rtti
    file    : file
    date    : (new Date()).toString()
    classes : json


  # write output file

  fs.writeFileSync "out.cpp", output


catch e

  console.log "[ERROR]".error, e

