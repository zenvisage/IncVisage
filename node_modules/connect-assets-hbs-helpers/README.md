# Connect-Assets Handlebars helpers

## Install

    npm install connect-assets-hbs-helpers

Simply `require 'connect-assets-hbs-helpers'` in your express server, and setup [connect-assets](https://github.com/adunkman/connect-assets) (check link for instructions)

## Usage

In your handlebars templates you can now use these helpers:

    {{{css 'horse' 'screen and (max-device-width: 480px)'}}}
    {{{js 'monkey'}}}
    {{{img 'screenshots/panda'}}}
