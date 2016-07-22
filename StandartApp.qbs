import qbs

CppApplication {
	Group {
		fileTagsFilter: product.type //удаляет лишнию строчку из узлов древа проекта
		qbs.install: true
	}
	Export {
		Depends { name: "cpp" }
		cpp.cppFlags: "-std=c++14"
		cpp.includePaths: product.sourceDirectory
		cpp.defines: ["USING_" + product.name.toUpperCase()]
	}
	Group {
		name: 'src'
		files: ["*.c", "*.h", "*.cpp"]
		prefix: "**/"
	}
}
