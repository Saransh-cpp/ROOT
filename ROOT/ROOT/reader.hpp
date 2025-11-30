#include <string>
#include <vector>

#include "config.hpp"

struct ReaderOptions {
    char sep = ',';                    // field separator (CSV): commonly ','
    char quote = '"';                  // quote/delimitor char: commonly '"'
    bool has_header = true;            // whether first row is a header row
    std::vector<std::string> headers;  // optional explicit header names (used if has_header == false)
    ReaderOptions() = default;
    explicit ReaderOptions(char sep_, char quote_, bool has_header_ = true)
        : sep(sep_), quote(quote_), has_header(has_header_) {}
};

class ReaderBase {
  public:
    std::string filename;
    ReaderOptions options;
    virtual std::unique_ptr<ConfigBase> read() = 0;
    ReaderBase(const std::string& filename, const ReaderOptions& options = ReaderOptions());

  protected:
    static std::string trim(const std::string& untrimmed_str);
    static bool parseBool(const std::string& bool_str, bool& out);
    static bool parseDouble(const std::string& double_str, double& out);
    static bool parseInt(const std::string& int_str, int& out);
    static bool parseMethod(const std::string& method_str, Method& out);
    static std::unique_ptr<ConfigBase> make_config_from_map(
        const std::unordered_map<std::string, std::string>& config_map);
};

class ReaderCSV : public ReaderBase {
  public:
    ReaderCSV(const std::string& filename, const ReaderOptions& options = ReaderOptions());
    std::unique_ptr<ConfigBase> read() override;

  private:
    std::vector<std::string> splitCsvLine(const std::string& line) const;
};

class ReaderDAT : public ReaderBase {
  public:
    ReaderDAT(const std::string& filename, const ReaderOptions& options = ReaderOptions());
    std::unique_ptr<ConfigBase> read() override;
};
