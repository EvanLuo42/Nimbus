#pragma once

#include <format>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace Nimbus::Utils::Error
{
class ArgParseError : public std::runtime_error
{
public:
    explicit ArgParseError(const std::string& msg) : std::runtime_error(std::format("Arg Parse Error: {}", msg)) {}
};
} // namespace Nimbus::Utils::Error

namespace Nimbus::Utils
{
class ArgParser
{
public:
    ArgParser(const int argc, char** argv)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (std::string arg = argv[i]; arg.starts_with("--"))
            {
                std::string key = arg.substr(2);
                std::string value;
                if (i + 1 < argc && !std::string(argv[i + 1]).starts_with("--"))
                {
                    value = argv[++i];
                }
                args[key] = value;
            }
        }
    }

    [[nodiscard]] bool has(const std::string& key) const { return args.contains(key); }

    [[nodiscard]] std::optional<std::string> getString(const std::string& key) const
    {
        if (const auto it = args.find(key); it != args.end() && !it->second.empty())
            return it->second;
        return std::nullopt;
    }

    [[nodiscard]] std::optional<int> getInt(const std::string& key) const
    {
        if (const auto it = args.find(key); it != args.end() && !it->second.empty())
        {
            try
            {
                return std::stoi(key);
            }
            catch (const std::invalid_argument&)
            {
                throw Error::ArgParseError(std::format("Invalid integer for '--{}': '{}'", key, it->second));
            }
            catch (const std::out_of_range&)
            {
                throw Error::ArgParseError(std::format("Integer out of range for '--{}'", key));
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] std::optional<float> getFloat(const std::string& key) const
    {
        if (const auto it = args.find(key); it != args.end() && !it->second.empty())
        {
            try
            {
                return std::stof(key);
            }
            catch (const std::invalid_argument&)
            {
                throw Error::ArgParseError(std::format("Invalid float for '--{}': '{}'", key, it->second));
            }
            catch (const std::out_of_range&)
            {
                throw Error::ArgParseError(std::format("Float out of range for '--{}'", key));
            }
        }
        return std::nullopt;
    }

private:
    std::unordered_map<std::string, std::string> args;
};
} // namespace Nimbus::Utils
