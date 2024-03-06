#include "EncryptionAlgorithms.hpp"

int main()
{
    setlocale(LC_CTYPE, "ukr");
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251); 

    std::string alphabet = "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ";
    
    CipherManager cipherManager{ alphabet };
    cipherManager.start();
}