#include "EncryptionAlgorithms.hpp"

int findMultiplicativeInversionKey(const CipherManager& cipherManager, int multKey)
{
    int invMultKey = -1;
    for (const auto& [symbol, number] : cipherManager.abc) {
        if (multKey * number % cipherManager.abc.size() == 1) {
            invMultKey = number;
            std::cout << "����������� ���������������� ���� - " << invMultKey << '\n';
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

std::string CaesarCipher::encryptPlainText(const CipherManager& cipherManager, std::string_view plainText)
{
    std::cout << "������ ���� ��� ����������: ";
    int key = 0;
    std::cin >> key;

    auto caesarExpressionForEncryption = [&](int symbolNumber, int alphabetSize) -> int {
        return static_cast<int>((symbolNumber + key) % alphabetSize);
    };
    std::string encryptedText = createTheResultingCipherString(caesarExpressionForEncryption, cipherManager, plainText);
    return encryptedText;
}
std::string CaesarCipher::decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText)
{
    std::cout << "������ ���� ��� ������������: ";
    int key = 0;
    std::cin >> key;

    auto caesarExpressionForDecryption = [&](int symbolNumber, int alphabetSize) -> int {
        return static_cast<int>((symbolNumber + alphabetSize - key) % alphabetSize);
    };
    std::string decryptedText = createTheResultingCipherString(caesarExpressionForDecryption, cipherManager, encryptedText);
    return decryptedText;
}
CaesarCipher::~CaesarCipher() { std::cout << "CaesarCipher object destroyed.\n"; };


std::string LinearCipher::encryptPlainText(const CipherManager& cipherManager, std::string_view plainText)
{
    int multKey = 0, invMultKey = 0;
    while (true)
    {
        std::cout << "������ ���� ��� ����������(4, 8, 10): ";
        std::cin >> multKey;
        invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
        if (invMultKey == -1) {
            std::cout << "����������� ���������������� ���� �� ��������. ����-����� ��������� ����� ����. \n";
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
std::string LinearCipher::decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText)
{
    int multKey = 0, invMultKey = 0;
    while (true)
    {
        std::cout << "������ ���� ��� ������������(4, 8, 10): ";
        std::cin >> multKey;
        invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
        if (invMultKey == -1) {
            std::cout << "����������� ���������������� ���� �� ��������. ����-����� ��������� ����� ����. \n";
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
LinearCipher::~LinearCipher() { std::cout << "LinearCipher object destroyed.\n"; };


std::string AffineCipher::encryptPlainText(const CipherManager& cipherManager, std::string_view plainText)
{
    int multKey = 0, invMultKey = 0, addKey = 0;
    while (true)
    {
        std::cout << "������ ����� ��� ����������((4 2), (8 4), (10 6)): ";
        std::cin >> multKey >> addKey;
        invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
        if (invMultKey == -1) {
            std::cout << "����������� ���������������� ���� �� ��������. ����-����� ��������� ����� ������ ����. \n";
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
std::string AffineCipher::decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText)
{
    int multKey = 0, invMultKey, addKey = 0;
    while (true)
    {
        std::cout << "������ ����� ��� ������������((4 2), (8 4), (10 6)): ";
        std::cin >> multKey >> addKey;
        invMultKey = findMultiplicativeInversionKey(cipherManager, multKey);
        if (invMultKey == -1) {
            std::cout << "����������� ���������������� ���� �� ��������. ����-����� ��������� ����� ������ ����. \n";
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
AffineCipher::~AffineCipher() { std::cout << "AffineCipher object destroyed.\n"; };


std::string PlayfairCipher::encryptPlainText(const CipherManager& cipherManager, std::string_view plainText)
{
    auto playfairExpressionForEncryption = [&](int symbolPosition, int matrixRowSize) -> int {
        return static_cast<int>((symbolPosition + 1) % matrixRowSize);
    };
    std::string encryptedText = createTheTransformedResultString(playfairExpressionForEncryption, cipherManager, plainText);

    return encryptedText;
}

std::string PlayfairCipher::decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText)
{
    auto playfairExpressionForDecryption = [&](int symbolPosition, int matrixRowSize) -> int {
        return static_cast<int>(((symbolPosition - 1) + matrixRowSize) % matrixRowSize);
    };
    std::string decryptedText = createTheTransformedResultString(playfairExpressionForDecryption, cipherManager, encryptedText);

    return decryptedText;
}

void PlayfairCipher::fillInTheMatrix(std::unordered_map<char, rowAndCol>& keyMatrix, std::vector<std::vector<char>>& additionalMatrix, std::string_view fillerStr, int& row, int& col, int matrixRowSize)
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
    std::cout << "������ ����-����� ��� ��������� ������� �������: ";
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
PlayfairCipher::~PlayfairCipher() { std::cout << "PlayfairCipher object destroyed.\n"; };


CipherManager::CipherManager(std::string_view alphabet_)
    :alphabet(alphabet_)
{
    for (const auto& symbol : alphabet) {
        abc[symbol] = static_cast<int>(abc.size());
    }
}
void CipherManager::start()
{
    bool next = true;
    while (next) {
        int cipherNumber = 0;
        while (true) {
            std::cout << "\n������� �������� ����(1 - ���� ������, 2 - ˳����� ����, 3 - ������� ����, 4 - Playfair): ";
            std::cin >> cipherNumber;
            if (cipherNumber == 1 || cipherNumber == 2 || cipherNumber == 3 || cipherNumber == 4) { break; }
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(32767, '\n');
            }
        }
        std::cout << "������� �����������(0) �� ������������(1) ?: ";
        bool action;
        std::cin >> action;
        std::cin.ignore();

        std::cout << "������ �����: ";
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
        std::cout << "���������: " << result << '\n';
        std::cout << "����������?(��� - 1, ͳ - 0): ";
        std::cin >> next;
    }
}
std::unique_ptr<ICipher> CipherManager::createCipherObject(int cipherNumber)
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