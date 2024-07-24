#include <iostream>
#include "observer.h"

int main() {
  std::shared_ptr<ConcreteSubject> sub = std::make_shared<ConcreteSubject>();
	
	std::shared_ptr<Observer> obs0 = std::make_shared<ConcreteObserver>(sub, "obs0");
	std::shared_ptr<Observer> obs1 = std::make_shared<ConcreteObserver>(sub, "obs1");
	
	sub->attach(obs0);
	sub->attach(obs1);
	
	{
		std::shared_ptr<Observer> obs2 = std::make_shared<ConcreteObserver>(sub, "obs2");
		sub->attach(obs2);
		sub->setState(1);       
	}
  std::cout << "-----------------------------------\n";

	sub->detach(obs0);
	
	std::shared_ptr<Observer> obs3 = std::make_shared<ConcreteObserver>(sub, "obs3");
	std::shared_ptr<Observer> obs4 = std::make_shared<ConcreteObserver>(sub);
	sub->attach(obs3);
	sub->attach(obs4);

	sub->setState(2);

	return 0;
}
