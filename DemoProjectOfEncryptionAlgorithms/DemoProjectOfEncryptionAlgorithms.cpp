#include <memory>
#include <locale>
#include <iostream>
#include <Windows.h>
#include <unordered_map>
#include <string_view>
#include <functional>
#include <string>
#include <cmath>

class CipherManager;
int findMultiplicativeInversionKey(const CipherManager& cipherManager, int multKey);
std::string createTheResultingCipherString(const std::function<int(int, int)>& cipherExpression, const CipherManager& cipherManager, std::string_view text);

class ICipher
{
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) = 0;
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) = 0;
    virtual ~ICipher() = default;
};

class CaesarCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override
    {
        std::cout << "Введіть ключ для шифрування: ";
        int key = 0;
        std::cin >> key;

        auto caesarExpressionForEncryption = [&](int symbolNumber, int alphabetSize) -> int {
            return static_cast<int>((symbolNumber + key) % alphabetSize);
        };
        std::string encryptedText = createTheResultingCipherString(caesarExpressionForEncryption, cipherManager, plainText);
        return encryptedText;
    }
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override
    {
        std::cout << "Введіть ключ для дешифрування: ";
        int key = 0;
        std::cin >> key;

        auto caesarExpressionForDecryption = [&](int symbolNumber, int alphabetSize) -> int {
            return static_cast<int>((symbolNumber + alphabetSize - key) % alphabetSize);
        };
        std::string decryptedText = createTheResultingCipherString(caesarExpressionForDecryption, cipherManager, encryptedText);
        return decryptedText;
    }
    virtual ~CaesarCipher() { std::cout << "CaesarCipher object destroyed.\n"; };
};

class LinearCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override
    {
        int multKey = 0, invMultKey = 0;
        while (true)
        {
            std::cout << "Введіть ключ для шифрування(4, 8, 10): ";
            std::cin >> multKey;
            invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший ключ. \n";
                continue;
            }
            break;
        }
        auto linearExpressionForEncryption = [&](int symbolNumber, int alphabetSize) -> int {
            return static_cast<int>((symbolNumber * multKey) % alphabetSize);
        };
        std::string encryptedText = createTheResultingCipherString(linearExpressionForEncryption, cipherManager, plainText);
        return encryptedText;
    }
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override
    {
        int multKey = 0, invMultKey = 0;
        while (true)
        {
            std::cout << "Введіть ключ для дешифрування(4, 8, 10): ";
            std::cin >> multKey;
            invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший ключ. \n";
                continue;
            }
            break;
        }
        auto linearExpressionForDecryption = [&](int symbolNumber, int alphabetSize) -> int {
            return static_cast<int>((invMultKey * symbolNumber) % alphabetSize);
        };
        std::string decryptedText = createTheResultingCipherString(linearExpressionForDecryption, cipherManager, encryptedText);
        return decryptedText;
    }
    virtual ~LinearCipher() { std::cout << "LinearCipher object destroyed.\n"; };
};

class AffineCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override
    {
        int multKey = 0, invMultKey = 0, addKey = 0;
        while (true)
        {
            std::cout << "Введіть ключі для шифрування((4 2), (8 4), (10 6)): ";
            std::cin >> multKey >> addKey;
            invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший перший ключ. \n";
                continue;
            }
            break;
        }
        auto affineExpressionForEncryption = [&](int symbolNumber, int alphabetSize) -> int {
            return static_cast<int>(((symbolNumber * multKey) + addKey) % alphabetSize);
        };
        std::string encryptedText = createTheResultingCipherString(affineExpressionForEncryption, cipherManager, plainText);
        return encryptedText;
    }
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override
    {
        int multKey = 0, invMultKey, addKey = 0;
        while (true)
        {
            std::cout << "Введіть ключі для дешифрування((4 2), (8 4), (10 6)): ";
            std::cin >> multKey >> addKey;
            invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
            if (invMultKey == -1) {
                std::cout << "Інверсивний мультиплікативний ключ не знайдено. Будь-ласка спробуйте інший перший ключ. \n";
                continue;
            }
            break;
        }
        auto affineExpressionForDecryption = [&](int symbolNumber, int alphabetSize) -> int {
            return static_cast<int>((invMultKey * (symbolNumber + alphabetSize - addKey)) % alphabetSize);
        };
        std::string decryptedText = createTheResultingCipherString(affineExpressionForDecryption, cipherManager, encryptedText);
        return decryptedText;
    }
    virtual ~AffineCipher() { std::cout << "AffineCipher object destroyed.\n"; };
};

class PlayfairCipher : public ICipher
{
private:
    struct rowAndCol { int row; int col; };
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override
    {
        auto playfairExpressionForEncryption = [&](int symbolPosition, int matrixRowSize) -> int {
            return static_cast<int>((symbolPosition + 1) % matrixRowSize);
        };
        std::string encryptedText = createTheTransformedResultString(playfairExpressionForEncryption, cipherManager, plainText);

        return encryptedText;
    }
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override
    {
        auto playfairExpressionForDecryption = [&](int symbolPosition, int matrixRowSize) -> int {
            return static_cast<int>(((symbolPosition - 1) + matrixRowSize ) % matrixRowSize);
        };
        std::string decryptedText = createTheTransformedResultString(playfairExpressionForDecryption, cipherManager, encryptedText);

        return decryptedText;
    }
    std::string createTheTransformedResultString(const std::function<int(int, int)>& transformationExpression, const CipherManager& cipherManager, std::string_view text_);
    void fillInTheMatrix(std::unordered_map<char, rowAndCol>& keyMatrix, std::vector<std::vector<char>>& additionalMatrix, std::string_view fillerStr, int& row, int& col, int matrixRowSize)
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
    virtual ~PlayfairCipher() { std::cout << "PlayfairCipher object destroyed.\n"; };
};

class CipherManager
{
public:
    CipherManager(std::string_view alphabet_)
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
                result = ciphers[cipherNumber]->decipherEncryptedText(*this, plainText);
            }
            else {
                result = ciphers[cipherNumber]->encryptPlainText(*this, plainText);
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
    friend std::string PlayfairCipher::createTheTransformedResultString(const std::function<int(int, int)>& transformationExpression, const CipherManager& cipherManager, std::string_view text_);
    friend int findMultiplicativeInversionKey(const CipherManager& cipherManager, int multKey);
    friend std::string createTheResultingCipherString(const std::function<int(int, int)>& cipherExpression, const CipherManager& cipherManager, std::string_view text);
private:
    std::unordered_map<char, int> abc;
    std::unordered_map<int, std::unique_ptr<ICipher>> ciphers;
    std::string_view alphabet;
};

int findMultiplicativeInversionKey(const CipherManager& cipherManager, int multKey)
{
    int invMultKey = -1;
    for (const auto& [symbol, number] : cipherManager.abc) {
        if (multKey * number % cipherManager.abc.size() == 1) {
            invMultKey = number;
            std::cout << "Інверсивний мультиплікативний ключ - " << invMultKey << '\n';
            break;
        }
    }
    return invMultKey;
}

std::string createTheResultingCipherString(const std::function<int(int, int)>& cipherExpression, const CipherManager& cipherManager, std::string_view text)
{
    std::string_view alphabet = cipherManager.alphabet;
    int alphabetSize = static_cast<int>(cipherManager.abc.size());
    std::string resultText = "";
    for (const auto& symbol : text)
    {
        if (cipherManager.abc.contains(symbol)) {
            int symbolNumber = cipherManager.abc.at(symbol);
            int shiftedPos = cipherExpression(symbolNumber, alphabetSize);
            resultText += alphabet[shiftedPos];
        }
        else {
            resultText += symbol;
        }
    }
    return resultText;
}

std::string PlayfairCipher::createTheTransformedResultString(const std::function<int(int, int)>& transformationExpression, const CipherManager& cipherManager, std::string_view text_)
{
    std::string_view alphabet = cipherManager.alphabet;
    std::string text {text_};
    if (text.size() % 2 != 0) {
        text += '.';
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

int main()
{
    setlocale(LC_CTYPE, "ukr");
    SetConsoleCP(1251); 
    SetConsoleOutputCP(1251); 

    std::string alphabet = "АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ";
    
    CipherManager cipherManager{ alphabet };
    cipherManager.start();
}