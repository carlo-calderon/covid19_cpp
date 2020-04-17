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
/*def matrix2Table(filename_in, row_name, column_name, value_name, filename_out) :
	f_writer = open(filename_out, 'w', newline = '', encoding = 'utf-8')
	writer = csv.writer(f_writer)
	writer.writerow([row_name, column_name, value_name])
	with open(filename_in, newline = '', encoding = 'utf-8') as f :
reader = csv.DictReader(f)
for row in reader :
if not(row_name in row.keys()) :
	continue
	for key in row.keys() :
		if key == row_name :
			continue
			writer.writerow([row[row_name], key, row[key]])
			f_writer.close()*/

int main(int argc, char* argv[]) {
	std::string path_products = "G:/Mi unidad/scripts/covid19/COVID19-Chile/output/";
	matrix2Table(path_products + "producto9/HospitalizadosUCIEtario.csv", "Grupo de edad", "Fecha", "UCI",
		"./temp/HospitalizadosUCIEtario.csv");
	return 0;
}