#include <cstdint>

namespace nhzaci {

template <typename TradeImpl>
concept TradeLike = requires(TradeImpl impl) {};

class Trade {
public:
  using price_t = uint64_t;
  using id_t = uint64_t;

private:
};

}; // namespace nhzaci