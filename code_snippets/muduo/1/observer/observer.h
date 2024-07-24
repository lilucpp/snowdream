#pragma once
#include <memory>
#include <list>
#include <iostream>
#include <string>

class Observer {
	public:
		Observer() {}
		virtual ~Observer(){}
	
		virtual void update()=0;
};

bool operator==(const std::weak_ptr<Observer> &lhs,
                const std::weak_ptr<Observer> &rhs) noexcept {
  return lhs.lock() == rhs.lock();
}

class Subject {
 public:
  Subject() {}
  virtual ~Subject() {}

  virtual void attach(std::shared_ptr<Observer> pob) = 0;
  virtual void detach(std::shared_ptr<Observer> pob) = 0;
  virtual void setState(int val) = 0;
  virtual int getState() = 0;
  virtual void notify() = 0;
};

class ConcreteObserver : public Observer {
	public:
		ConcreteObserver(std::shared_ptr<Subject> psub, const std::string& name="unknow name")
			: psub_(psub), name_(name), state_(0) {}
		~ConcreteObserver(){}

		void update() override { 
			state_ = psub_->getState();
			printInfo();
		}

	private:
		void printInfo() { std::cout << name_ << " recive: " << state_ << std::endl; }
	
	private:
		std::shared_ptr<Subject> psub_;
		int state_;
		std::string name_;
};

class ConcreteSubject : public Subject {
 public:
  ConcreteSubject() : state_(0) {}
  ~ConcreteSubject() {}

  void attach(std::shared_ptr<Observer> pob) override {
    observers_.push_back(pob);
  }
  void detach(std::shared_ptr<Observer> pob) override {
    observers_.remove(pob);
  }
  int getState() override { return state_; }

  void setState(int val) override {
    state_ = val;
    notify();
  }

  void notify() override {
    for (auto pob : observers_) {
      if (auto spob = pob.lock()) spob->update();
    }
  }

 private:
  int state_;
  std::list<std::weak_ptr<Observer>> observers_;
};
