#pragma once

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
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override;
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override;
    virtual ~CaesarCipher();
};

class LinearCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override;
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override;
    virtual ~LinearCipher();
};

class AffineCipher : public ICipher
{
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override;
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override;
    virtual ~AffineCipher();
};

class PlayfairCipher : public ICipher
{
private:
    struct rowAndCol { int row; int col; };
public:
    virtual std::string encryptPlainText(const CipherManager& cipherManager, std::string_view plainText) override;
    virtual std::string decipherEncryptedText(const CipherManager& cipherManager, std::string_view encryptedText) override;
    std::string createTheTransformedResultString(const std::function<int(int, int)>& transformationExpression, const CipherManager& cipherManager, std::string_view text_);
    void fillInTheMatrix(std::unordered_map<char, rowAndCol>& keyMatrix, std::vector<std::vector<char>>& additionalMatrix, std::string_view fillerStr, int& row, int& col, int matrixRowSize);
    virtual ~PlayfairCipher();
};

class CipherManager
{
public:
    CipherManager(std::string_view alphabet_);
    void start();
    std::unique_ptr<ICipher> createCipherObject(int cipherNumber);
    ~CipherManager() = default;

    friend std::string PlayfairCipher::createTheTransformedResultString(const std::function<int(int, int)>& transformationExpression, const CipherManager& cipherManager, std::string_view text_);
    friend int findMultiplicativeInversionKey(const CipherManager& cipherManager, int multKey);
    friend std::string createTheResultingCipherString(const std::function<int(int, int)>& cipherExpression, const CipherManager& cipherManager, std::string_view text);
private:
    std::unordered_map<char, int> abc;
    std::unordered_map<int, std::unique_ptr<ICipher>> ciphers;
    std::string_view alphabet;
};


