#include "..\include\json\json.h"
#include <fstream>
#include <iostream>
#include <shlobj.h>
#include<filesystem>

namespace fs = std::filesystem; 
using namespace std;

int main()
{
	int width, height;
	int input;
	cin >> input;
	if (input > 2)
		input = 0;

	wchar_t* localAppData = new wchar_t[128];
	HRESULT hr =
		SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &localAppData);
	char sLocalAppPath[128];
	size_t copy_count = 0;
	wcstombs_s(&copy_count, sLocalAppPath, 128, localAppData, 128);
	std::wstringstream str_json_file;
	str_json_file << localAppData << "\\Acer\\SpatialLabs\\Display Service\\generation_settings.json";

	ifstream ifs;
	ifs.open(str_json_file.str().c_str());

	Json::Reader reader;
	Json::Value root;
	Json::Value data;

	if (ifs.is_open()) {
		Json::CharReaderBuilder builder;
		builder["collectComments"] = false;
		JSONCPP_STRING errs;
		if (parseFromStream(builder, ifs, &root, &errs)) {
			data = root["SLDSSettingMode"][to_string(input)];
			if (!data.isNull()) {
				width = data["ModelInputWidth"].asUInt();
				height = data["ModelInputHeight"].asUInt();
			}
		}
	}
	else {
		cout << "Cant find file, Creating... " << endl;
		std::wstringstream str_json_dir;
		fs::path str_json_path;
		str_json_dir << localAppData << "\\Acer\\SpatialLabs\\Display Service";
		str_json_path = str_json_dir.str().c_str();

		auto ret = fs::create_directories(str_json_path);

		root["SLDSSettingMode"]["0"]["ModelInputWidth"] = 352;
		root["SLDSSettingMode"]["0"]["ModelInputHeight"] = 192;
		root["SLDSSettingMode"]["1"]["ModelInputWidth"] = 672;
		root["SLDSSettingMode"]["1"]["ModelInputHeight"] =384;
		root["SLDSSettingMode"]["2"]["ModelInputWidth"] = 1376;
		root["SLDSSettingMode"]["2"]["ModelInputHeight"] = 768;
		Json::StyledWriter sw;
		ofstream os;
		os.open(str_json_file.str().c_str());
		os << sw.write(root);
		os.close();
	}
	cout << "width: " << width << endl;
	cout << "height: " << height << endl;

	return 0;
}