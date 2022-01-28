PROJECT_GENERATOR_VERSION = 3
include("third-party/garrysmod_common")

CreateWorkspace { name = "rnlib" }
	CreateProject { serverside = false }
		IncludeHelpersExtended()
		IncludeSDKTier0()
		IncludeSDKTier1()
		IncludeSDKCommon()
		IncludeDetouring()

		includedirs  "third-party/"
		files {"source/*.cpp", "source/*.hpp", "third-party/**.*"}