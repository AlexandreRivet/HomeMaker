# TestMod gui init module
# (c) 2001 Juergen Riegel LGPL

class TestModWorkbench ( Workbench ):
	"TestMod workbench object"
	MenuText = "TestMod"
	ToolTip = "TestMod workbench"
	def Initialize(self):
		# load the module
		import TestModGui
	def GetClassName(self):
		return "TestModGui::Workbench"

Gui.addWorkbench(TestModWorkbench())
