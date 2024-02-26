#include <memory>
#include <iostream>
#include <locale>
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <functional>
#include <cmath>

int findMultiplicativeInversionKey(std::unordered_map<char, int>& abc, int multKey)
{
    int invMultKey = -1;
    for (const auto& [symbol, number] : abc) {
        if (multKey * number % abc.size() == 1) {
            invMultKey = number;
            std::cout << "Інверсивний мультиплікативний ключ - " << invMultKey << '\n';
            break;
        }
    }
    return invMultKey;
}

std::string createTheResultingCipherString(const std::function<int(int)>& cipherExpression, std::unordered_map<char, int>& abc, std::string text, std::string alphabet)
{
    std::string resultText = "";
    for (const auto& symbol : text)
    {
        if (abc.contains(symbol)) {
            int symbolNumber = abc[symbol];
            int shiftedPos = cipherExpression(symbolNumber);
            resultText += alphabet[shiftedPos];
        }
        else {
            resultText += symbol;
        }
    }
    return resultText;
}

class ICipher
{
public:
    virtual std::string encryptPlainText(std::unordered_map<char, int>& abc, std::string plainText, std::string alphabet) = 0;
    virtual std::string decipherEncryptedText(std::unordered_map<char, int>& abc, std::string encryptedText, std::string alphabet) = 0;
    virtual ~ICipher() = default;
};

class CaesarCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(std::unordered_map<char, int>& abc, std::string plainText, std::string alphabet) override
    {
        std::cout << "Введіть ключ для шифрування: ";
        int key = 0;
        std::cin >> key;

        auto caesarExpressionForEncryption = [&](int symbolNumber) -> int {
            return static_cast<int>((symbolNumber + key) % abc.size());
        };
        std::string encryptedText = createTheResultingCipherString(caesarExpressionForEncryption, abc, plainText, alphabet);
        return encryptedText;
    }
    virtual std::string decipherEncryptedText(std::unordered_map<char, int>& abc, std::string encryptedText, std::string alphabet) override
    {
        std::cout << "Введіть ключ для дешифрування: ";
        int key = 0;
        std::cin >> key;

        auto caesarExpressionForDecryption = [&](int symbolNumber) -> int {
            return static_cast<int>((symbolNumber + abc.size() - key) % abc.size());
        };
        std::string decryptedText = createTheResultingCipherString(caesarExpressionForDecryption, abc, encryptedText, alphabet);
        return decryptedText;
    }
    virtual ~CaesarCipher() { std::cout << "CaesarCipher object destroyed.\n"; };
};

class LinearCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(std::unordered_map<char, int>& abc, std::string plainText, std::string alphabet) override
    {
        int multKey = 0, invMultKey = 0;
        while (true)
        {
            std::cout << "Введіть ключ для шифрування(4, 8, 10): ";
            std::cin >> multKey;
            invMultKey = findMultiplicativeInversionKey(abc, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший ключ. \n";
                continue;
            }
            break;
        }
        auto linearExpressionForEncryption = [&](int symbolNumber) -> int {
            return static_cast<int>((symbolNumber * multKey) % abc.size());
        };
        std::string encryptedText = createTheResultingCipherString(linearExpressionForEncryption, abc, plainText, alphabet);
        return encryptedText;
    }
    virtual std::string decipherEncryptedText(std::unordered_map<char, int>& abc, std::string encryptedText, std::string alphabet) override
    {
        int multKey = 0, invMultKey = 0;
        while (true)
        {
            std::cout << "Введіть ключ для дешифрування(4, 8, 10): ";
            std::cin >> multKey;
            invMultKey = findMultiplicativeInversionKey(abc, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший ключ. \n";
                continue;
            }
            break;
        }
        auto linearExpressionForDecryption = [&](int symbolNumber) -> int {
            return static_cast<int>((invMultKey * symbolNumber) % abc.size());
        };
        std::string decryptedText = createTheResultingCipherString(linearExpressionForDecryption, abc, encryptedText, alphabet);
        return decryptedText;
    }
    virtual ~LinearCipher() { std::cout << "LinearCipher object destroyed.\n"; };
};

class AffineCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(std::unordered_map<char, int>& abc, std::string plainText, std::string alphabet) override
    {
        int multKey = 0, invMultKey = 0, addKey = 0;
        while (true)
        {
            std::cout << "Введіть ключі для шифрування((4 2), (8 4), (10 6)): ";
            std::cin >> multKey >> addKey;
            invMultKey = findMultiplicativeInversionKey(abc, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший перший ключ. \n";
                continue;
            }
            break;
        }
        auto affineExpressionForEncryption = [&](int symbolNumber) -> int {
            return static_cast<int>(((symbolNumber * multKey) + addKey) % abc.size());
        };
        std::string encryptedText = createTheResultingCipherString(affineExpressionForEncryption, abc, plainText, alphabet);
        return encryptedText;
    }
    virtual std::string decipherEncryptedText(std::unordered_map<char, int>& abc, std::string encryptedText, std::string alphabet) override
    {
        int multKey = 0, invMultKey, addKey = 0;
        while (true)
        {
            std::cout << "Введіть ключі для дешифрування((4 2), (8 4), (10 6)): ";
            std::cin >> multKey >> addKey;
            invMultKey = findMultiplicativeInversionKey(abc, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший перший ключ. \n";
                continue;
            }
            break;
        }
        auto affineExpressionForDecryption = [&](int symbolNumber) -> int {
            return static_cast<int>((invMultKey * (symbolNumber + abc.size() - addKey)) % abc.size());
        };
        std::string decryptedText = createTheResultingCipherString(affineExpressionForDecryption, abc, encryptedText, alphabet);
        return decryptedText;
    }
    virtual ~AffineCipher() { std::cout << "AffineCipher object destroyed.\n"; };
};


class PlayfairCipher : public ICipher
{
private:
    struct rowAndCol { int row; int col; };
public:
    virtual std::string encryptPlainText(std::unordered_map<char, int>& abc, std::string plainText, std::string alphabet) override
    {
        auto playfairExpressionForEncryption = [&](int symbolPosition, int matrixRowSize) -> int {
            return static_cast<int>((symbolPosition + 1) % matrixRowSize);
        };
        std::string encryptedText = createTheTransformedResultString(playfairExpressionForEncryption, plainText, alphabet);

        return encryptedText;
    }
    virtual std::string decipherEncryptedText(std::unordered_map<char, int>& abc, std::string encryptedText, std::string alphabet) override
    {
        auto playfairExpressionForDecryption = [&](int symbolPosition, int matrixRowSize) -> int {
            return static_cast<int>(((symbolPosition - 1) + matrixRowSize ) % matrixRowSize);
        };
        std::string decryptedText = createTheTransformedResultString(playfairExpressionForDecryption, encryptedText, alphabet);

        return decryptedText;
    }
    std::string createTheTransformedResultString(const std::function<int(int, int)>& transformationExpression, std::string text, const std::string& alphabet)
    {
        if (text.size() % 2 != 0) {
            text += 'Х';
        }
        int matrixMaxSize = 36, row = 0, col = 0;
        int matrixRowSize = static_cast<int>(sqrt(matrixMaxSize));
        std::string fillers = "_,.-<>/()*&^%$#@!{}:;?|+=";

        std::vector<std::vector<char>> additionalMatrix;
        additionalMatrix.resize(matrixRowSize);
        for (auto& matrixRow : additionalMatrix) {
            matrixRow.resize(matrixRowSize);
        }
        std::unordered_map<char, rowAndCol> keyMatrix;
        std::string keyPhrase;
        std::cout << "Введіть ключ-фразу для створення ключової матриці: ";
        getline(std::cin, keyPhrase);

        fillInTheMatrix(keyMatrix, additionalMatrix, keyPhrase, row, col, matrixRowSize);
        fillInTheMatrix(keyMatrix, additionalMatrix, alphabet, row, col, matrixRowSize);
        fillInTheMatrix(keyMatrix, additionalMatrix, fillers, row, col, matrixRowSize);

        std::string encryptedText = "";

        for (int i = 0; i < text.size(); i += 2) {
            const auto& firstSymbolLocation = keyMatrix[text[i]];
            const auto& secondSymbolLocation = keyMatrix[text[i + 1]];

            char firstShiftedSymbol = text[i];
            char secondShiftedSymbol = text[i + 1];

            if (firstSymbolLocation.row == secondSymbolLocation.row) {
                int shiftedColPosOfTheFirstSymbol = transformationExpression(firstSymbolLocation.col, matrixRowSize);
                int shiftedColPosOfTheSecondSymbol = transformationExpression(secondSymbolLocation.col, matrixRowSize);

                firstShiftedSymbol = additionalMatrix[firstSymbolLocation.row][shiftedColPosOfTheFirstSymbol];
                secondShiftedSymbol = additionalMatrix[secondSymbolLocation.row][shiftedColPosOfTheSecondSymbol];
            }
            else if (firstSymbolLocation.col == secondSymbolLocation.col) {
                int shiftedRowPosOfTheFirstSymbol = transformationExpression(firstSymbolLocation.row, matrixRowSize);
                int shiftedRowPosOfTheSecondSymbol = transformationExpression(secondSymbolLocation.row, matrixRowSize);

                firstShiftedSymbol = additionalMatrix[shiftedRowPosOfTheFirstSymbol][firstSymbolLocation.col];
                secondShiftedSymbol = additionalMatrix[shiftedRowPosOfTheSecondSymbol][secondSymbolLocation.col];
            }
            else {
                firstShiftedSymbol = additionalMatrix[firstSymbolLocation.row][secondSymbolLocation.col];
                secondShiftedSymbol = additionalMatrix[secondSymbolLocation.row][firstSymbolLocation.col];
            }
            encryptedText += firstShiftedSymbol;
            encryptedText += secondShiftedSymbol;
        }
        return encryptedText;
    }
    void fillInTheMatrix(std::unordered_map<char, rowAndCol>& keyMatrix, std::vector<std::vector<char>>& additionalMatrix, const std::string& fillerStr, int& row, int& col, int matrixRowSize)
    {
        for (const auto& symbol : fillerStr) {
            if (keyMatrix.size() == std::pow(matrixRowSize, 2)) return;
            if (!keyMatrix.contains(symbol)) {
                keyMatrix[symbol] = { .row = row, .col = col };
                additionalMatrix[row][col] = symbol;
                if (col == matrixRowSize - 1) {
                    std::cout << symbol << '(' << row << ',' << col << ')' << '\n';
                    col = 0;
                    ++row;
                    continue;
                }
                std::cout << symbol << '(' << row << ',' << col << ')' << ' ';
                ++col;
            }
        }
    }
};

class CipherManager
{
public:
    CipherManager(std::string alphabet_)
        :alphabet(alphabet_)
    {
        for (const auto& symbol : alphabet) {
            abc[symbol] = static_cast<int>(abc.size());
        }
    }
    void start()
    {
        bool next = true;
        while (next) {
            int cipherNumber = 0;
            while (true) {
                std::cout << "\nВиберіть потрібний шифр(1 - шифр Цезаря, 2 - Лінійний шифр, 3 - Афінний шифр, 4 - Playfair): ";
                std::cin >> cipherNumber;
                if (cipherNumber == 1 || cipherNumber == 2 || cipherNumber == 3 || cipherNumber == 4) { break; }
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(32767, '\n');
                }
            }
            std::cout << "Потрібно зашифрувати(0) чи розшифрувати(1) ?: ";
            bool action;
            std::cin >> action;
            std::cin.ignore();

            std::cout << "Введіть текст: ";
            std::string plainText;
            getline(std::cin, plainText);

            if (!ciphers.contains(cipherNumber)) {
                ciphers[cipherNumber] = createCipherObject(cipherNumber);
            }
            std::string result = "";
            if (action) {
                result = ciphers[cipherNumber]->decipherEncryptedText(abc, plainText, alphabet);
            }
            else {
                result = ciphers[cipherNumber]->encryptPlainText(abc, plainText, alphabet);
            }
            std::cout << "Результат: " << result << '\n';
            std::cout << "Продовжуємо?(Так - 1, Ні - 0): ";
            std::cin >> next;
        }
    }
    std::unique_ptr<ICipher> createCipherObject(int cipherNumber)
    {
        std::unique_ptr<ICipher> cipherObject = nullptr;
        switch (cipherNumber)
        {
        case 1:
            cipherObject = std::make_unique<CaesarCipher>();
            break;
        case 2:
            cipherObject = std::make_unique<LinearCipher>();
            break;
        case 3:
            cipherObject = std::make_unique<AffineCipher>();
            break;
        case 4:
            cipherObject = std::make_unique<PlayfairCipher>();
            break;
        }
        return cipherObject;
    }
    ~CipherManager() {}
private:
    std::unordered_map<char, int> abc;
    std::unordered_map<int, std::unique_ptr<ICipher>> ciphers;
    std::string alphabet;
};

int main()
{
    setlocale(LC_CTYPE, "ukr");
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251); 

    std::string alphabet = "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ";

    CipherManager cipherManager{ alphabet };
    cipherManager.start();
}
