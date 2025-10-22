
# Abstract

During every day workflows, accidents happen. Due to this, we need a way to undo what was done to go back to an acceptable version of your file or program state. For this reason, the IO model of Foundations ***needs*** to have a way to keep track of what was done. 

# General System Layout

The core class of the Timeline system is the Timeline object, an instance based object that keeps track of all of the actions preformed on a FileInstance. For this reason, each FileInstance has a Timeline instance attached to it, and they are bound in a similar sense to the AddressBook object. 

The Timeline object has properties that control how the Timeline works and store the data in the timeline action. The Timeline only stores the Current position in the timeline. This is represented by a pointer reference to an [[ActionBase]] object, of which that object knows what its previous and next actions are, as well as its top parameter (see more in [[ActionBase]]). This system allows the Timeline object to advance to the next action (redo), recede to the previous action (undo), and branch (undo, then preform a change). 

Branching is a feature of the system that allows the Timeline to keep multiple versions of the workflow, but the memory usage is yet to be known. For this reason, the Timeline object has the ability to not branch the Main Timeline. For more on branching, see the [[Branching]] article. 

# General Terminology

1. Push - Add an item to the end of the current branche's last action.
2. Advance - Move to the next item from the current action.
3. Recede - Move to the previous item from the current action.
4. Action Shift - Change locations in the Timeline. Advancing and Receding are both Action Shifts.

# Actions

The TImeline class uses the ActionBase class to store information about what was done to the FileInstance. Whenever something is changed in the file, the information is posted to the Timeline's current node, adding it to be the next item in the action and advancing the Timeline to the new ActionBase. 

## Undo
When the user calls on the Timeline to preform an undo, the Timeline simply recedes to the previous ActionBase, by setting the Current equal to the ActionBase's Previous property. 

## Pushing an Action while in an Undo
If an action is posted to the Timeline *when the Current node's next parameter is not nullptr (it is not at the end of the list)* The current actions after the current node will either be pushed to a branch or deleted all together. 

The action that the Timeline preforms is based on if the Timeline has branching enabled. If it does, the changes are pushed on to a branch which is linked to the curent node, and then the new action is posted to the Current node. After this, the Timeline will advance to the new node.

## Redo
When the user calls on the Timeline to preform a Redo, it attempts to move to the next item after the Current parameter. If the Next parameter of the Current is equal to nullptr, the redo is aborted. If not, the Timeline advances to that node. 

# Saving

Timelines are able to be saved in a file format. Like the AddressBook, they also have the ability to load select portions of their saved data into memory, saving on memory and overall processing time. Typically, the Timeline will start out with no actions loaded into memory. As the file progresses, the timeline will be constructed in memory. When the file saves, the Timeline object will append the information saved in memory to the file. This way the entire action history of the file can be preserved, and even if the program is closed and re-opened, actions can be undone. 

To be able to undo actions that are not stored in memory, the Timeline must be requested to load that information from memory. To determine which item to load, the Actions have IDs attached to them. They can be represented with two numbers: one for storing the Actions position in the branch, and the other is to determine the branch. The branch ID is assigned by the Timeline, and the position ID is calculated by the Actions position relative to the start of the branch. Position 0 in the Action's position means it is the first action of the file, and a Branch ID of 0 means it is the Main branch. 

After a decent number of actions, there comes a time that the information no longer needs to be saved into memory. Because of this, the Timeline will notate where it should stop keeping information saved into memory. This is based off of the count of items in memory. When the FileInstance is saved again, the Timeline will push the data out of memory and into the file. This helps handle memory better. If the amount of items to be pushed to the file is greater than 2 times the file saving's threshold, it will push a request to the IO saving thread to save the file in the background and free up the timeline. 