import "../StandartApp.qbs" as StandartApp

StandartApp {
	Depends { name: "UtilityLib" }
	cpp.dynamicLibraries: "pthread"
}
