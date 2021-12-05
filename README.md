# Portal 2 32 player mod plugin

This plugin is specifically meant as an addition to @kyleraykbs 32 player mod for Portal 2. It currently has very limited functionality but I have plans to expand soon.

Current features:
- concommand: get_username, returns username of player at index (1-32 i think?)  
- vscript function `GetPlayerName`, same as above, just a binding for vscript  
- vscript function `AddChatCallback`, provided with a string of a callback function x, it will call it simularly to `x(userid, "message text")`  

If you got any issues, feel free to report, also seeking windows build maintainers since i kinda don't want to spend my time on setting up a cross compiler  
