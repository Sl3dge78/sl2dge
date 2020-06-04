# sl2dge
Welcome to the simple lightweight 2d game engine!

This engine that I'm working on during my free time doesn't have any ambition other than being coding practice for me.  

It's built in cpp, using SDL and is inspired by rpg maker in the way to make games with it.  

There are 3 projects in the repo :  
- engine : core of the engine, has its main meat  
- map_editor : a tiled map editor useable with the engine  
- game : an example game that i'm working on as i develop the engine  

Here's basically how it works :

In your main : 
- Create an instance of Game. 
- Push your a State to Game with PushState.

Create a child class of GameState :
- This is where you'll put your code.
- Use your GameState to load maps, instantiate the player, ...
- Add an EventManager and add GameEvents to it to create your game logic. 
