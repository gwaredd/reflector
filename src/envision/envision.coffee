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

  console.log "Usage:".cyan, "#{ coffee } #{ script }", "<json file> <output cpp>".yellow
  process.exit()


#-------------------------------------------------------------------------------

processJson = (file) ->

  console.log ""
  console.log "Processing '#{ file }'"

  return require file


#-------------------------------------------------------------------------------

try

  usage() unless process.argv.length > 3

  # load json and templates

  filein  = process.argv[ 2 ]
  fileout = process.argv[ 3 ]
  json    = processJson filein
  tmpl    = dot.process path: "."

  # run templates with json

  output  = tmpl.rtti
    file    : filein
    date    : (new Date()).toString()
    classes : json


  # write output file

  fs.writeFileSync fileout, output


catch e

  console.log "[ERROR]".error, e

