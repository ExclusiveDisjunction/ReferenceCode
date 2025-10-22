
## Prerequisites 

To understand which control to write, please see [[Control & UI Base Classes]]. This will let you know if you need a SingleChildControl, ChildlessControl, or a MultiChildControl. Please also see [[Styles & Visual States]] to know how the render engine for the UI works. 


## General Concept

In some instances, there are collections of controls that just need to go together for the UI to have a good meaning. This can be achived with a repetative script, such as a loop. However, if you need to hold onto each handle of each control for data extraction/displaying, this can become very ineficent. It would be more practical to keep all of the controls together, and also automate the data extraction/displaying process. This can be achived with custom controls. 

Custom controls are special controls that are defined by the programmer that branch out from the Common Controls. The Common Controls are good for simple use, but in more complex situations, this is not very practial, as stated above. This guide shows how to make a custom control. 

However, before spending a lot of time making a custom control, please see the list of [[Common Controls]] to make sure that there isn't a control already that does what you want. 

## First Step, Picking a Base Class

## Second Step, Determining your Properties

## Third Step, Filling in your automation

## Fourth Step, Placing your Control in the UI

