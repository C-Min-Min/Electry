package com.example.c__app;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class MeasureAdapter extends RecyclerView.Adapter<MeasureAdapter.MeasureViewHolder> {

    private Context mCtx;
    private List<Measure> measureList;

    public MeasureAdapter(Context mCtx, List<Measure> measureList) {
        this.mCtx = mCtx;
        this.measureList = measureList;
    }

    @NonNull
    @Override
    public MeasureViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(mCtx);
        View view = inflater.inflate(R.layout.measure_list, null);
        return new MeasureViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull MeasureViewHolder holder, int position) {
        Measure measure = measureList.get(position);

        holder.textViewId.setText("Id: " + String.valueOf(measure.getId()));
        holder.textViewStart_Time.setText("Start_time: " + String.valueOf(measure.getStart_time()));
        holder.textViewPower.setText("Power: " + String.valueOf(measure.getPower()));
    }

    @Override
    public int getItemCount() {
        return measureList.size();
    }

    public class MeasureViewHolder extends RecyclerView.ViewHolder {
        TextView textViewId, textViewStart_Time, textViewPower;

        public MeasureViewHolder(View itemView){
            super(itemView);

            textViewId = itemView.findViewById(R.id.textViewId);
            textViewStart_Time = itemView.findViewById(R.id.textViewStart_Time);
            textViewPower = itemView.findViewById(R.id.textViewPower);
        }
    }
}
