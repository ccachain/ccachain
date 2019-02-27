#include "try_handle.hpp"

namespace kafka {

void handle(std::function<void()> handler, const std::string& desc) {
    try {
        handler();
    } catch (fc::exception& e) {
        elog("FC Exception while ${desc}: ${e}", ("e", e.to_string())("desc", desc));
        std::_Exit(1);
    } catch (std::exception& e) {
        elog("STD Exception while ${desc}: ${e}", ("e", e.what())("desc", desc));
        std::_Exit(1);
    } catch (...) {
        elog("Unknown exception while ${desc}", ("desc", desc));
        std::_Exit(1);
    }
}

}
