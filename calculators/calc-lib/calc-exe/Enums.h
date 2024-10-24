#pragma once

namespace Mason
{
	enum AppStates
	{
		APS_None = 0,
		APS_FileLoaded = 1,
		APS_HasEdit = 2,
		APS_ReadOnly = 8,
		APS_EditorOpen = 16,
		APS_AppendableEditorRunning = 32
	};
}