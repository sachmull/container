test_vector:
	c++ -Wall -Wextra -std=c++98 test_vector.cpp
	@./a.out > ft
	@c++ test_vector.cpp -D NS=std
	@./a.out > std
	@diff std ft > diff

docker:
	docker run -ti -v $PWD:/app memory-test  bash
