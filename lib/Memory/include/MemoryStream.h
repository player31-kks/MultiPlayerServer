#pragma once


#include <cstdint>
#include <vector>
#include <string>
#include <span>
#include <concepts>
#include <type_traits>
#include <memory>

class OutputMemoryStream
{
public:
    OutputMemoryStream()
    {
        ReallocBuffer(32);
    }

    ~OutputMemoryStream() = default;

    [[nodiscard]] std::span<const std::byte> GetBuffer() const noexcept
    {
        return std::span<const std::byte>(
            reinterpret_cast<const std::byte*>(mBuffer.get()), mHead
        );
    }

    [[nodiscard]] std::uint32_t GetLength() const noexcept { 
        return mHead; 
    }

    void Write(std::span<const std::byte> inData)
    {
        const auto inByteCount = inData.size_bytes();
        if (mHead + inByteCount > mCapacity)
        {
            ReallocBuffer(mHead + inByteCount);
        }
        std::copy_n(inData.data(), inByteCount, mBuffer.get() + mHead);
        mHead += static_cast<std::uint32_t>(inByteCount);
    }

    template <typename T>
        requires std::is_arithmetic_v<T> || std::is_enum_v<T>
    void Write(T inData)
    {
        if constexpr (STREAM_ENDIANNESS == PLATFORM_ENDIANNESS)
        {
            Write(std::as_bytes(std::span(&inData, 1)));
        }
        else
        {
            T swappedData = ByteSwap(inData);
            Write(std::as_bytes(std::span(&swappedData, 1)));
        }
    }

    void Write(const std::vector<int>& inIntVector)
    {
        Write(inIntVector.size());
        Write(std::as_bytes(std::span(inIntVector)));
    }

    template <typename T>
    void Write(const std::vector<T>& inVector)
    {
        Write(static_cast<std::uint32_t>(inVector.size()));
        for (const auto& element : inVector)
        {
            Write(element);
        }
    }

    void Write(std::string_view inString)
    {
        Write(inString.size());
        Write(std::as_bytes(std::span(inString)));
    }

   

private:
    void ReallocBuffer(std::uint32_t inNewLength)
    {
        std::unique_ptr<std::byte[]> newBuffer = std::make_unique<std::byte[]>(inNewLength);
        if (mBuffer)
        {
            std::copy_n(mBuffer.get(), mHead, newBuffer.get());
        }
        mBuffer = std::move(newBuffer);
        mCapacity = inNewLength;
    }

    std::unique_ptr<std::byte[]> mBuffer;
    std::uint32_t mHead = 0;
    std::uint32_t mCapacity = 0;

};
