﻿#include <iostream>
#include <string>
#include <Windows.h>
#include <shellapi.h>
#include <fstream>
#include <stdio.h>
#include <urlmon.h>
#include "firmware.cpp"
#pragma comment(lib, "urlmon.lib")

using namespace std;

const string futurerestore_url = "https://github.com/s0uthwest/futurerestore/releases/download/245/futurerestore_win64_v245.zip";
const string zip_file = "C:\\Users\\Ev\\Desktop\\fr.zip";
const string futurerestore = "C:\\Users\\Ev\\Desktop\\futurerestore.exe";
const string ipsw = "C:\\Users\\Ev\\Desktop\\ipsw.ipsw";

inline void remove_leftover(string file) {
    if (remove(file.c_str()) == 0) cout << "Leftovers are removed successfully!" << endl;
    else cout << "Deleting leftovers failed! You may need to delete them manually." << endl;
}

inline void download(const string& version) {
    if (file_exist(futurerestore) == false) {
        wstring url_temp = wstring(futurerestore_url.begin(), futurerestore_url.end());
        LPCWSTR url = url_temp.c_str();

        wstring dir_temp = wstring(zip_file.begin(), zip_file.end());
        LPCWSTR destination = dir_temp.c_str();

        cout << "Downloading futurerestore... " << endl;
        HRESULT hr = URLDownloadToFile(NULL, url, destination, 0, NULL);
        if (hr != 0) {
            cout << "Futurerestore download failed! Aborting.." << endl;
            system("pause");
            exit(0);
        }
    }
    else
        cout << "Futurerestore already exists on desktop, continuing.." << endl;
    if (file_exist(ipsw) == false) {
        string download_link = get_firmware_link(version);
        wstring url_temp = wstring(download_link.begin(), download_link.end());
        LPCWSTR url = url_temp.c_str();

        wstring dir_temp = wstring(ipsw.begin(), ipsw.end());
        LPCWSTR destination = dir_temp.c_str();

        cout << "Downloading iOS " << version << " for iPhone 5s... " << endl;
        HRESULT hr = URLDownloadToFile(NULL, url, destination, 0, NULL);
        if (file_exist(ipsw) == true)
            cout << "iOS " << version << " firmware file for iPhone 5s is downloaded, continuing.." << endl;
        else {
            cout << "Firmware file download failed! Aborting.." << endl;
            system("pause");
            exit(0);
        }
    }
    else
        cout << "Firmware file already exists on desktop, continuing.." << endl;
}

inline void prep(string _sig_hash, string _fw_file, const string& version) {
    ofstream exec("C:\\Users\\Ev\\Desktop\\run.bat");
    exec << "cd C:\\Program Files\\7-Zip" << "\n7z e " << "\"" << zip_file << "\"" << " -o\"C:\\Users\\Ev\\Desktop\\\"";
    if (!(version == "10.2" || version == "10.2.1" || version == "10.3" || version == "10.3.1" || version == "10.3.2" || version == "10.3.3")) {
        exec << "\ncd C:\\Users\\Ev\\Desktop\\" << "\nfuturerestore.exe -t " << _sig_hash << " --latest-sep --latest-baseband " << _fw_file << "\npause";
        exec.close();
    }
    else {
        exec << "\ncd C:\\Users\\Ev\\Desktop\\" << "\nfuturerestore.exe -t " << _sig_hash << " -b baseband.bbfw -p buildmanifest.plist -s sep.im4p -m buildmanifest.plist ";
        exec << _fw_file << "\npause";
        exec.close();
    }
    ostream flush();
    cout << "Preparation is done!" << endl;
}

inline void run() {
    ShellExecuteA(
        0,
        "open",
        "C:\\Users\\Ev\\Desktop\\run.bat",
        NULL,
        NULL,
        SW_SHOW);
    system("pause");
    remove_leftover("C:\\Users\\Ev\\Desktop\\run.bat");
    remove_leftover("C:\\Users\\Ev\\Desktop\\fr.zip");
    remove_leftover("C:\\Users\\Ev\\Desktop\\futurerestore.exe");
}

int main()
{
    string version;
    cout << "Which version you'd like to restore: ";
    getline(cin, version);

    prep("blob.shsh2", "ipsw.ipsw", version);
    download(version);
    version_control(version);
    run();

    system("pause");
    return 0;
}
