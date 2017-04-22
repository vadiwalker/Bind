#ifndef BIND_H
#define BIND_H

#include <tuple>
#include <iostream>


namespace BIND {
	
	template <std::size_t N>
	struct placeholder {};

	template <typename Func, typename... Args>
	struct my_bind {

		template <typename... NewArgs>
		auto operator() (NewArgs&&... newArgs) {
			return invoker(typename build_inds<std::tuple_size< std::tuple<Args...> >::value>::type(), std::forward<NewArgs>(newArgs)...);
		}
	
		my_bind(Func&& func, Args &&... args) 
			: func(std::forward<Func>(func)),
				args(std::forward<Args>(args)...) {
						//std::cerr << std::tuple_size< std::tuple<Args...> >::value << "\n";
					}
	
	private:

		template <std::size_t... Indices>
		struct indices {};

		template <std::size_t N, std::size_t... Indices>
		struct build_inds {
			typedef typename build_inds<N-1, N-1, Indices...>::type type;	
		};

		template <std::size_t... Indices>
		struct build_inds<0, Indices...> {
			typedef indices<Indices...> type;
		};
	
		template <typename T, typename... NewArgs>
		auto&& subs(T&& val, NewArgs&&... newArgs) {
			return val;
		}

		template <typename OtherFunc, typename... OtherArgs, typename... NewArgs>
		auto subs(my_bind<OtherFunc, OtherArgs...>& argBind, NewArgs &&... newArgs) {
			return argBind(std::forward<NewArgs>(newArgs)...);
		}

		template <std::size_t Index, typename... NewArgs>
		auto&& subs(placeholder<Index>, NewArgs&&... newArgs) {
			return std::get<Index>(std::forward_as_tuple(newArgs...));
		}

		template <std::size_t... Indices, typename... NewArgs>
		auto invoker(indices<Indices...> inds, NewArgs&&... newArgs) {
			return func(subs(std::get<Indices>(args), std::forward<NewArgs>(newArgs)...)...);
		}

		typename std::decay<Func>::type func;
		std::tuple <typename std::decay<Args>::type...> args;

	};

	template <typename Func, typename... Args>
	my_bind<Func, Args...> bind(Func&& func, Args &&... args) {
		return my_bind<Func, Args...> (std::forward<Func>(func), std::forward<Args>(args)...);
	}

	constexpr placeholder<0> __1;
	constexpr placeholder<1> __2;
	constexpr placeholder<2> __3;
	constexpr placeholder<3> __4;
}
#endif
