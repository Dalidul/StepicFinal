import "../StandartApp.qbs" as StandartApp

StandartApp {
	condition: true
	//cpp.defines: ['LOG_TEST_OUT', ]
	Depends { name: "HttpServer" }
	Depends { name: "UtilityLib" }
	Depends { name: "Qt"; submodules: ["core", "test", ] }
}
