#include <iostream>
#include <vector>
#include <fstream>
#include "rapidcsv/src/rapidcsv.h"

using namespace std;

void matrix2Table(const std::string& filename_in, const std::string& row_name,
	const std::string& column_name, const std::string& value_name, const std::string& filename_out) {
	rapidcsv::Document doc(filename_in);
	std::ofstream f_out(filename_out);
	std::vector<std::string> header = doc.GetColumnNames();
	f_out << row_name << "," << column_name << "," << value_name << std::endl;
	for (size_t i = 0; i < doc.GetRowCount(); ++i) {
		for (size_t j = 0; j < doc.GetColumnCount(); ++j) {
			f_out << doc.GetRowName(i) << "," << doc.GetColumnName(j) << "," << doc.GetCell<std::string>(j, i) << std::endl;
		}
	}
	f_out.close();
}

int main(int argc, char* argv[]) {
	std::string path_products = "G:/Mi unidad/scripts/covid19/COVID19-Chile/output/";
	matrix2Table(path_products + "producto9/HospitalizadosUCIEtario.csv", "Grupo de edad", "Fecha", "UCI",
		"./temp/HospitalizadosUCIEtario.csv");
	matrix2Table(path_products + "producto10/FallecidosEtario.csv", "Grupo de edad", "Fecha", "Fallecidos",
		"./temp/FallecidosEtario.csv");
	return 0;
}