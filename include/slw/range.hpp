#ifndef SLW_RANGE_HPP
#define SLW_RANGE_HPP

#include <ranges>
#include <algorithm>

namespace slw {

namespace ranges {

    using namespace std::ranges;

    #ifndef __cpp_lib_ranges_generate_random

        template <typename R, typename G>
            requires output_range<R, std::invoke_result_t<G&>> &&
            std::uniform_random_bit_generator<std::remove_cvref<G>>
        constexpr borrowed_iterator_t<R> generate_random(R&& r, G&& g)
        {
            for (auto& e : r) {
                e = g();
            }
        }

    #endif // __cpp_lib_ranges_generate_random

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

    #endif // __cpp_lib_ranges_enumerate

} // namespace views

} // namespace slw

#endif // SLW_RANGE_HPP