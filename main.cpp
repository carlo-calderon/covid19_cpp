#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "rapidcsv/src/rapidcsv.h"

using namespace std;
namespace fs = std::experimental::filesystem;

typedef std::map<std::string, int> DataDetail;
typedef std::map<std::string, DataDetail> DataRegion;
typedef std::map<std::string, DataRegion> DataCovid;
typedef std::map<std::string, std::string> Tags;

DataDetail data_empty = { {"cases", 0}, {"deaths", 0}, {"recovered", 0}, {"cases_acc", 0}, {"deaths_acc", 0},
{"recovered_acc", 0}, {"pcr", 0}, {"uci", 0} };

/* Transforma un csv de matriz en un csv de tabla, la tabla es el prodcuto cruz entre la primera columna y la primera fila*/
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
/* Transforma un csv de matriz en un csv de tabla, la tabla es el prodcuto cruz entre la primeras columnas que estan en la lista columnames y la primera fila*/
void matrix2Table(const std::string& filename_in, const std::vector<std::string>& row_names,
	const std::string& column_name, const std::string& value_name, const std::string& filename_out) {
	rapidcsv::Document doc(filename_in);
	std::ofstream f_out(filename_out);
	std::vector<std::string> header = doc.GetColumnNames();
	for (size_t i = 0; i < row_names.size(); ++i)
		f_out << row_names[i] << ",";
	f_out << column_name << "," << value_name << std::endl;
	for (size_t i = 0; i < doc.GetRowCount(); ++i) {
		for (size_t j = 0; j < doc.GetColumnCount(); ++j) {
			if (j < row_names.size() - 1)
				continue;
			f_out << doc.GetRowName(i) << ",";
			for (size_t ir = 1; ir < row_names.size(); ++ir)
				f_out << doc.GetCell<std::string>(ir - 1, i) << ",";
			f_out << doc.GetColumnName(j) << "," << doc.GetCell<std::string>(j, i) << std::endl;
		}
	}
	f_out.close();
}

void fillDataDetail(const std::string& filename, DataCovid& data, const std::string& fecha, const Tags& tags) {
	std::cout << fecha << ", " << filename << std::endl;
	rapidcsv::Document doc(filename);
	std::vector<std::string> columnNames = doc.GetColumnNames();
	for (size_t i = 0; i < doc.GetRowCount(); ++i) {
		std::string region = doc.GetRowName(i);
		if (region.length()==0 || region == "Total")
			continue;
		if (data.count(region) == 0) {
			data.insert({ region, DataRegion()});
		}
		if (data[region].count(fecha) == 0) {
			data[region].insert({ fecha, DataDetail(data_empty) });
		}
		for (Tags::const_iterator it = tags.cbegin(); it != tags.cend(); ++it) {
			if (std::find(columnNames.begin(), columnNames.end(), it->first) != columnNames.end()) {
				if (doc.GetCell<std::string>(it->first, region).length() > 0) {
					int cell = doc.GetCell<int>(it->first, region);
					data[region][fecha][it->second] = cell;
				}
			}
		}
	}
}

void fillDifferences(DataCovid& data, const std::string& column, const std::string& column_dif) {
	for (DataCovid::iterator it_r = data.begin(); it_r != data.end(); ++it_r) {
		std::string region = it_r->first;
		for (DataRegion::iterator it_d = it_r->second.begin(); it_d != it_r->second.end(); ++it_d) {
			DataRegion::iterator it_d_ant = it_d;
			it_d_ant--;
			if (it_d == it_r->second.begin())
				continue;
			int data_actual = it_d->second[column];
			int data_ant = it_d_ant->second[column];
			it_d->second[column_dif] = data_actual - data_ant;
		}
	}
}

void writeDataCovid(const std::string& filename, DataCovid& data) {
	std::ofstream f_out(filename);
	f_out << "dateRep,day,month,year,cases,deaths,recovered,cases_acc,deaths_acc,recovered_acc,Region,popData2018,pcr,uci" << std::endl;
	for (DataCovid::iterator it_r = data.begin(); it_r != data.end(); ++it_r) {
		std::string region = it_r->first;
		for (DataRegion::iterator it_d = it_r->second.begin(); it_d != it_r->second.end(); ++it_d) {
			std::string fecha = it_d->first;
			f_out << fecha.substr(8, 2) << "-" << fecha.substr(5, 2) << "-" << fecha.substr(0, 4) << ",";
			f_out << stoi(fecha.substr(8, 2)) << "," << stoi(fecha.substr(5, 2)) << "," << fecha.substr(0, 4) << ",";
			f_out << data[region][fecha]["cases"] << "," << data[region][fecha]["deaths"] << "," << data[region][fecha]["recovered"] << ",";
			f_out << data[region][fecha]["cases_acc"] << "," << data[region][fecha]["deaths_acc"] << "," << data[region][fecha]["recovered_acc"] << ",";
			f_out << region << ",0,";
			f_out << data[region][fecha]["pcr"] << "," << data[region][fecha]["uci"] << std::endl;
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
	std::vector<std::string> row_names = { "Grupo de edad", "Sexo" };
	matrix2Table(path_products + "producto16/CasosGeneroEtario.csv", row_names, "Fecha", "Cases",
		"./temp/CasosGeneroEtario.csv");
	matrix2Table(path_products + "producto20/NumeroVentiladores.csv", "Ventiladores", "Fecha", "Cantidad",
		"./temp/NumeroVentiladores.csv");

	Tags tags = {
		{"Casos totales", "cases_acc"},
		{"Fallecidos", "deaths_acc"},
		{"Casos recuperados", "recovered_acc" } };
	DataCovid data_cl;

	for (auto& it : fs::directory_iterator("G:/Mi unidad/scripts/covid19/tmp/cl_producto4/")) {
		std::string fecha = it.path().filename().string().substr(0, 10);
		fillDataDetail(it.path().string(), data_cl, fecha, tags);
	}
	
	fillDifferences(data_cl, "cases_acc", "cases");
	fillDifferences(data_cl, "deaths_acc", "deaths");
	fillDifferences(data_cl, "recovered_acc", "recovered");

	writeDataCovid("./output/covid19_cl.csv", data_cl);

	return 0;
}