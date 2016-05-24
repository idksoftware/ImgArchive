program hooks.
program hooks are hooks into the application that allow you to change and expand how the program behaves. these come in the form of small scripts that are called at important times as the application carries out actions. for example, when the application imports images there is a pre-import hook, this allow you to perform any action on the image just before importing into the archive. a normal way to use this hook is to call external programs such as lmagemagic to create sidebar images or rotate an image before importation.

image archive scripts
these are xml text files that control custom actions. These actions can perform a number of common tasks such as calling the xsl processor, copying files, calling external programs and scripts.

external scripts
external scripts can be called directly or from a xml script. on calling an external script image archive will export an environment that the external program or script can use to comunicate with image archive. these are called environment variables. environment variables are a common way to pass information to scripts both windows and UNIX (both mac and linux are forms of UNIX).

image archive enables you to control how images are imported by using the import hooks.

xml scripts
xml scripts start with a start tag of y<ScriptHook Name="Pre-Import" Description="Rotates
