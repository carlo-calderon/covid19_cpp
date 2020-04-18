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

void fillDataDetail(const std::string& filename, DataCovid& data, const std::string& fecha, const Tags& tags) {
	std::cout << fecha << ", " << filename << std::endl;
	rapidcsv::Document doc(filename);
	for (size_t i = 0; i < doc.GetRowCount(); ++i) {
		std::string region = doc.GetRowName(i);
		if (region == "Total")
			continue;
		if (data.count(region) == 0) {
			data.insert({ region, DataRegion()});
		}
		if (data[region].count(fecha) == 0) {
			data[region].insert({ fecha, DataDetail(data_empty) });
		}
		for (Tags::const_iterator it = tags.cbegin(); it != tags.cend(); ++it) {

			//if(doc.GetCell())
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
/*
	with open(filename, newline='', encoding='utf-8') as f:
		reader = csv.DictReader(f)
		for row in reader:
			try:
				if not(row['Region'] in data_cl.keys()):
					if row['Region'] == 'Total':
						continue
					data_cl[row['Region']] = dict()
				if not(fecha in data_cl[row['Region']].keys()):
					data_cl[row['Region']][fecha] = dict(data_empty)
				for tag in tags:
					if tag in row.keys():
						if len(row[tag])>0:
							n=int(row[tag])
						else:
							n=0
						data_cl[row['Region']][fecha][tags[tag]] = n
			except:
				print('ERROR:', row.keys())*/
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

	Tags tags = {
		{"Casos totales", "cases_acc"},
		{"Fallecidos", "deaths_acc"},
		{"Casos recuperados", "recovered_acc" } };
	DataCovid data_cl;

	for (auto& it : fs::directory_iterator("G:/Mi unidad/scripts/covid19/tmp/cl_producto4/")) {
		std::string fecha = it.path().filename().string().substr(0, 10);
		fillDataDetail(it.path().string(), data_cl, fecha, tags);
	}

	writeDataCovid("./output/covid19_cl.csv", data_cl);

	return 0;
}