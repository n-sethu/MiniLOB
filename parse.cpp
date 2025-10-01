#include <fstream>  // For file stream operations (ifstream)
#include <iostream> // For input/output operations
#include <sstream>
#include <stdexcept>
#include <string> // For string manipulation
#include <unordered_map>
#include <vector> // To store parsed data
#include <map>
#include <fstream>

using namespace std;

enum class EventType { LIMIT_ADD, CANCEL, TRADE };
enum class Side { BUY, SELL };

struct OrderEvent {
  long long timestamp; // nanoseconds or microseconds
  EventType type;
  Side side;
  double price;
  int quantity;
};
// Create the graph based on the distance between the bid ask spread

struct LimitOrderBook {
  map<double, int> bids; // descending by price
  map<double, int> asks; // ascending by price
};

struct LOBRow {
  std::string system_time;
  double midpoint, spread;
  int buys, sells;
  std::vector<double> bids_distance;
  std::vector<double> bids_notional;
  std::vector<double> asks_distance;
  std::vector<double> asks_notional;
};


void write_metrics_csv(const std::vector<LOBRow> &rows,
                       const std::string &filename) {
  std::ofstream out(filename);
  out << "system_time,midpoint,spread,rel_spread,imbalance,weighted_bid_"
         "distance\n";

  for (const auto &row : rows) {
    double rel_spread = row.spread / row.midpoint;

    double bid_sum = 0, ask_sum = 0;
    for (auto v : row.bids_notional)
      bid_sum += v;
    for (auto v : row.asks_notional)
      ask_sum += v;

    double imbalance = (bid_sum - ask_sum) / (bid_sum + ask_sum);

    double weighted_bid_dist = 0.0, bid_norm = 0.0;
    for (size_t i = 0; i < row.bids_distance.size(); i++) {
      weighted_bid_dist += row.bids_distance[i] * row.bids_notional[i];
      bid_norm += row.bids_notional[i];
    }
    weighted_bid_dist /= bid_norm;

    out << row.system_time << "," << row.midpoint << "," << row.spread << ","
        << rel_spread << "," << imbalance << "," << weighted_bid_dist << "\n";
  }
}



int main() {
  string f = "/Users/nikhilsethu1/Coding/LOBPractice/HFTCryptoLOBdata.csv";
  ifstream inputFile;
  inputFile.open(f);
  if (!inputFile.is_open()) {
        cerr << "Could not find file: HFTCryptoLOBdata.csv\n";
    return 1;
}

  string line;
  getline(inputFile, line); // header
  vector<string> labels;
  string token;
  istringstream tokenStream(line);
  while (getline(tokenStream, token, ',')) {
    labels.push_back(token);
  }

  unordered_map<string, int> index;
  for (int i = 0; i < (int)labels.size(); i++) {
    index[labels[i]] = i;
  }

  vector<LOBRow> dataset;

  while (getline(inputFile, line)) {
    istringstream rowStream(line);
    vector<string> values;
    string value;
    while (getline(rowStream, value, ',')) {
      values.push_back(value);
    }

    LOBRow row;
    row.system_time = values[index["system_time"]];
    row.midpoint = stod(values[index["midpoint"]]);
    row.spread = stod(values[index["spread"]]);
    row.buys = stoi(values[index["buys"]]);
    row.sells = stoi(values[index["sells"]]);

    // Parse dynamic vectors
    for (int i = 0; i <= 14; i++) {
      row.bids_distance.push_back(
          stod(values[index["bids_distance_" + to_string(i)]]));
      row.bids_notional.push_back(
          stod(values[index["bids_notional_" + to_string(i)]]));
      row.asks_distance.push_back(
          stod(values[index["asks_distance_" + to_string(i)]]));
      row.asks_notional.push_back(
          stod(values[index["asks_notional_" + to_string(i)]]));
    }

    dataset.push_back(row);
  }

  inputFile.close();

  // Write processed metrics
  write_metrics_csv(dataset, "metrics_output.csv");
  cout << "Wrote metrics to metrics_output.csv\n";

}