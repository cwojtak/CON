package com.connorwojtak.derp;

public class Derp {
	private String name;

	public Derp(String name){
		this.name = name;
		int a = 0;
		while(a <= 10){
			if(a == 5){
				a = 7;
			}
			a++;
		}
	}
	
	public static void main(String args[]){
		Derp derp = new Derp("abc");
	}
}