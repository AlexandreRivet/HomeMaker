# HomeMaker gui init module
# (c) 2014 Alexandre Rivet
class HomeMakerWorkbench ( Workbench ):
	"HomeMaker workbench object"
	MenuText = "HomeMaker"
	ToolTip = "HomeMaker workbench"
			
	def Initialize(self):
		# load the module
		import HomeMakerGui
	def GetClassName(self):
		return "HomeMakerGui::Workbench"

Gui.addWorkbench(HomeMakerWorkbench())

# Probably append the open handler here, check this later