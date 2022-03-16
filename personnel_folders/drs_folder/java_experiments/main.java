package main;

import java.lang.Thread;
import java.util.ArrayList;

class Queue{
	
	private ArrayList<Integer> queue_in_storage;
	
	Queue(){
		queue_in_storage = new ArrayList<Integer>();
	}
	
	public void add_to_queue(int input) {
		queue_in_storage.add(input);
	}
	
	public void service_queue() {
		if(queue_in_storage.size() > 0) {
			queue_in_storage.remove(0);
		}
	}
	
	public void get_queue_size() {
		System.out.println(queue_in_storage.size());
	}
	
}

class thread extends Thread{
	public Queue queue;
	public int while_loop_counter = 0;
	
	void setup_queue() {
		queue = new Queue();
	}
	
	void set_queue(Queue shared_queue) {
		queue = shared_queue;
	}
	
	public void run() {
		while(while_loop_counter<100) {
			queue.service_queue();
			System.out.print("Items in Queue: ");  queue.get_queue_size();
			while_loop_counter++;
		}
	}

}

public class main {

	public static void main(String[] args) {
		thread transmission_thread = new thread();
		Queue queue_demo = new Queue(); 
		transmission_thread.setup_queue();
		transmission_thread.set_queue(queue_demo);
		transmission_thread.start();
		int iterator = 0;
		while(iterator<100) {
			queue_demo.add_to_queue(iterator);
			iterator++;
			transmission_thread.set_queue(queue_demo);
			if(iterator%5==0) {
				System.out.println("Iterator: " + iterator);
			}
		}
		
	}

}
