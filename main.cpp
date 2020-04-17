#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "rapidcsv/src/rapidcsv.h"

using namespace std;

typedef std::map<std::string, int> DataDetail;
typedef std::map<std::string, DataDetail> DataRegion;
typedef std::map<std::string, DataRegion> DataCovid;
typedef std::map<std::string, std::string> Tags;

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

void fillDataDetail(const std::string& filename, const DataCovid& data, const std::string& fecha, const Tags& tags) {

}
/*for f in listdir(path_p4) :
	fecha = f[0:10]
	print(f, fecha)
	fillDataDetalleCl(path_p4 + f, data_cl, fecha,
		{ 'Casos totales': 'cases_acc', 'Fallecidos' : 'deaths_acc', 'Casos recuperados' : 'recovered_acc' })*/
int main(int argc, char* argv[]) {
	std::string path_products = "G:/Mi unidad/scripts/covid19/COVID19-Chile/output/";
	matrix2Table(path_products + "producto9/HospitalizadosUCIEtario.csv", "Grupo de edad", "Fecha", "UCI",
		"./temp/HospitalizadosUCIEtario.csv");
	matrix2Table(path_products + "producto10/FallecidosEtario.csv", "Grupo de edad", "Fecha", "Fallecidos",
		"./temp/FallecidosEtario.csv");
	return 0;
}