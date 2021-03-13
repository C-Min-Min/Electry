package com.example.c__app;

public class Measure {
    private int id;
    private String start_time;
    private double power;

    public Measure(int id, String start_time, double power){
        this.id = id;
        this.start_time = start_time;
        this.power = power;
    }

    public int getId() {
        return id;
    }
    public String getStart_time() {
        return start_time;
    }
    public double getPower() {
        return power;
    }
}
