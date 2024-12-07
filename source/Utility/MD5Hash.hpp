#include <cstdint>
#include <string>
#include <vector>
#include <span>
#include <array>

std::array<std::byte, 8> uint64_to_bytes(uint64_t);
uint64_t bytes_to_uint64(std::span<std::byte, 8>);

class MD5_String
{
public:
    MD5_String(const std::string& string);

    size_t GetStringLength() const;
    size_t GetDataLength() const;

    const std::byte& At(size_t index) const;

    void Append(const std::string& str);
    void Resize(size_t new_message_size);

private:
    std::vector<std::byte> buffer;
};

union Hash128
{
    std::array<uint8_t, 16> as_bytes;
    std::array<uint32_t, 4> as_uints;
    std::array<uint64_t, 2> as_ulongs;
};

Hash128 MD5Hash(const MD5_String& input);

std::vector<uint8_t> PadMessage(const std::vector<uint8_t>& str);
std::string FormatHash(const Hash128& hash);
void MD5Transform(std::array<uint32_t, 4>& state, const uint8_t* buffer);
std::vector<uint8_t> StringToBytes(const std::string& str);

std::string OldHash(std::string);
