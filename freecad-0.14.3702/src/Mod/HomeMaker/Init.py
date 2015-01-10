# FreeCAD init script of the HomeMaker module
# (c) 2014 Alexandre Rivet 

# Get the Parameter Group of this module
ParGrp = App.ParamGet("System parameter:Modules").GetGroup("HomeMaker")

#Set the needed information
ParGrp.SetString("HelpIndex", "HomeMaker/Help/index.html")
ParGrp.SetString("WorkBenchName", "HomeMaker")
ParGrp.SetString("WorkBenchModule", "HomeMakerWorkebench.py")