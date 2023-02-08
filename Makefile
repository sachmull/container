NAME = ft_containers

$(NAME): all

all:
	@make test_vector
	@make test_stack
	@make test_map

test_vector:
	@c++ -Wall -Wextra -Werror -std=c++98 test_vector.cpp -o ft_vector
	@./ft_vector > ft_vector_out
	@c++ test_vector.cpp -D NS=std -o std_vector
	@./std_vector > std_vector_out
	@- diff std_vector_out ft_vector_out > diff_vector

test_stack:
	@c++ -Wall -Wextra -Werror -std=c++98 test_stack.cpp -o ft_stack
	@./ft_stack > ft_stack_out
	@c++ test_stack.cpp -D NS=std -o std_stack
	@./std_stack > std_stack_out
	@- diff std_stack_out ft_stack_out > diff_stack

test_map:
	@c++ -Wall -Wextra -Werror -std=c++98 test_map.cpp -o ft_map
	@./ft_map > ft_map_out
	@c++ test_map.cpp -D NS=std -o std_map
	@./std_map > std_map_out
	@ - diff std_map_out ft_map_out > diff_map

clean:
	@rm -rf ft_vector std_vector ft_vector_out std_vector_out diff_vector
	@rm -rf ft_stack std_stack ft_stack_out std_stack_out diff_stack
	@rm -rf ft_map std_map ft_map_out std_map_out diff_map

fclean: clean

re: rm all
