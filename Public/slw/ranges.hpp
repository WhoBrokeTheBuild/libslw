#ifndef SLW_RANGES_HPP
#define SLW_RANGES_HPP

#include <ranges>
#include <algorithm>

namespace slw {

namespace ranges {

    using namespace std::ranges;

} // namespace ranges

namespace views {
    
    using namespace std::views;

    #ifndef __cpp_lib_ranges_enumerate

        template <typename Iterable>
        class _enumerator
        {
        public:

            _enumerator(Iterable iterable)
                : _index(0)
                , _iterable(iterable)
                , _iterator(std::begin(iterable))
                , _end(std::end(iterable))
            { }

            inline const _enumerator& begin() const {
                return *this;
            }

            inline const _enumerator& end() const {
                return *this;
            }

            inline bool operator!=(const _enumerator&) const {
                return _iterator != _end;
            }

            inline void operator++() {
                ++_iterator;
                ++_index;
            }

            inline auto operator*() const {
                return std::pair(_index, *_iterator);
            }

        private:

            std::size_t _index;

            Iterable _iterable;

            decltype(std::begin(_iterable)) _iterator;

            const decltype(std::end(_iterable)) _end;

        };

        template <typename Iterable>
        inline auto enumerate(Iterable&& iterable) {
            return _enumerator<Iterable>(std::forward<Iterable>(iterable));
        }

    #endif

} // namespace views

} // namespace slw

#endif // SLW_RANGES_HPP