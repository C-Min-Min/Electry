package com.example.c__app;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.android.volley.toolbox.StringRequest;

import java.util.List;

public class HistoryAdapter extends RecyclerView.Adapter<HistoryAdapter.HistoryViewHolder> {

    private Context mCtx;
    private List<History> historyList;

    public HistoryAdapter(Context mCtx, List<History> historyList) {
        this.mCtx = mCtx;
        this.historyList = historyList;
    }

    @NonNull
    @Override
    public HistoryAdapter.HistoryViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        LayoutInflater inflater = LayoutInflater.from(mCtx);
        View view = inflater.inflate(R.layout.history_list, null);
        return new HistoryViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull HistoryAdapter.HistoryViewHolder holder, int position) {
        History history = historyList.get(position);

        holder.textViewTable_Name.setText(String.valueOf(history.getTable_name()));
    }

    @Override
    public int getItemCount() { return historyList.size(); }

    public class HistoryViewHolder extends RecyclerView.ViewHolder {
        TextView textViewTable_Name;

        public HistoryViewHolder(View itemView) {
            super(itemView);

            textViewTable_Name = itemView.findViewById(R.id.textViewTable_Name);
        }
    }
}
