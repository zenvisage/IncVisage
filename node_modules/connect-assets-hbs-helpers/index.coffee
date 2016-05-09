hbs = require 'express-hbs'

safeImg = (path) ->
  try
    img(path)
  catch error
    "/img/missing.png"

hbs.registerHelper 'css', (file, media) ->
  # By default second arg is context if not explicitly passed
  media = "all" unless media.substring?
  """<link href="#{css(file)}" media="#{media}" rel="stylesheet" type="text/css"/>"""

hbs.registerHelper 'js', (path) ->
  files = js(path)
  if files.substring?
    files = [files]
  ("""<script src="#{file}" type="text/javascript"></script>""" for file in files).join('\n')

hbs.registerHelper 'img', (file, options) ->
  throw "You must pass a path to an image" unless file
  path = safeImg(file)
  imgOptions = options.hash
  attributes = ("#{key}=\"#{val}\" " for key, val of imgOptions).join('')

  unless process.env.NODE_ENV is 'production'
    path += "?#{new Date().getTime()}"
  """<img src="#{path}" #{attributes}/>"""

hbs.registerHelper 'imgPath', (file) ->
  path = safeImg(file)
  unless process.env.NODE_ENV is 'production'
    path += "?#{new Date().getTime()}"
  path