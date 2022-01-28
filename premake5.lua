PROJECT_GENERATOR_VERSION = 3
include("third-party/garrysmod_common")

CreateWorkspace { name = "jellyfish" }
	CreateProject { serverside = false }
		IncludeHelpersExtended()
		IncludeSDKTier0()
		IncludeSDKTier1()
		IncludeSDKCommon()
		IncludeDetouring()
		IncludeLuaShared()
		IncludeScanning()
		IncludeSteamAPI()

		files {"source/*.cpp", "source/*.hpp"}