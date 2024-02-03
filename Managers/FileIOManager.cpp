#include "FileIOManager.hpp"

bool comparator(const std::string&, const std::string&);

FileIOManager::FileIOManager(int maxRecords) : 
    dir_("app_data/data/"), maxRecords_(maxRecords)
{
    file_ = dir_ + "records.ltx";

    if(!fs::exists(dir_)) 
        fs::create_directory(dir_);
}

std::vector<std::string> FileIOManager::readRecordsFromFile() const
{
    std::vector<std::string> records;

    std::fstream recordsFile(file_, ib::in | ib::binary);
    if(recordsFile.is_open())
    {
        std::string record;
        while(std::getline(recordsFile, record))
        {
            records.push_back(record);
        }

        recordsFile.close();
    }
    else
    {
        std::fstream createFile(file_, ib::out);
        createFile.close();
    }

    return records;
}

void FileIOManager::writeRecordsToFile(const std::vector<std::string>& records) const
{
    auto fileData = readRecordsFromFile();
    for(const auto& record : records)
        fileData.emplace_back(record);

    std::sort(fileData.begin(), fileData.end(), comparator);

    std::fstream recordsFile(file_, ib::out | ib::binary);
    if(recordsFile.is_open())
    {
        for(int i = 0; i < maxRecords_; i++)
            recordsFile << fileData[i] << std::endl;

        recordsFile.close();
    }
    else
    {
        std::cerr << "Unable to write file!\n";
    }
}

bool comparator(const std::string& time1, const std::string& time2)
{
    auto timeParser = [](const std::string& time)
    {
        int colon1 = time.find(":");
        int colon2 = time.rfind(":");

        int hours = std::stoi(time.substr(0, colon1));
        int minutes = std::stoi(time.substr(colon1 + 1, 2));
        int seconds = std::stoi(time.substr(colon2 + 1));

        return hours * 3600 + minutes * 60 + seconds;
    };

    return timeParser(time1) < timeParser(time2);
}
