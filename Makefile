test_vector:
	@c++ -Wall -Wextra -std=c++98 test_vector.cpp
	@./a.out > ft_vector
	@c++ test_vector.cpp -D NS=std
	@./a.out > std_vector
	@diff std_vector ft_vector > diff_vector

test_stack:
	@c++ -Wall -Wextra -std=c++98 test_stack.cpp
	@./a.out > ft_stack
	@c++ test_stack.cpp -D NS=std
	@./a.out > std_stack
	@diff std_stack ft_stack > diff_stack

docker:
	docker run -ti -v $PWD:/app memory-test  bash
