package com.example.c__app;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.android.volley.AuthFailureError;
import com.android.volley.NetworkResponse;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.UrlRewriter;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {

    String URL_MEASURES = "http://192.168.88.241/php_api/GET.php";
    String URL_TABLES = "http://192.168.88.241/php_api/GET_TABLE.php";

    List<Measure> measureList;
    List<History> historyList;
    LinearLayout linearLayout;
    RecyclerView recyclerView;
    Button b;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.history);

        linearLayout = findViewById(R.id.buttonlayout);
        linearLayout.setOrientation(LinearLayout.VERTICAL);
        measureList = new ArrayList<>();
        historyList = new ArrayList<>();

        loadHistory();

        /*Button button = (Button) findViewById(R.id.textViewTable_Name);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, button.getText().toString(), Toast.LENGTH_SHORT).show();
            }
        });*/

    }

    private void loadHistory() {
        StringRequest stringRequest = new StringRequest(Request.Method.GET, URL_TABLES,

                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {
                        try {

                            JSONArray array = new JSONArray(response);

                            for (int i = 0; i < array.length(); i++) {
                                JSONObject history = array.getJSONObject(i);
                                Button btn = new Button(MyApplication.getAppContext());
                                btn.setText(history.getString("0"));
                                LinearLayout linearLayout = (LinearLayout)findViewById(R.id.buttonlayout);
                                LinearLayout.LayoutParams buttonlayout = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                                linearLayout.addView(btn, buttonlayout);
                                btn.setOnClickListener(new View.OnClickListener()
                                {
                                    public void onClick(View view)
                                    {
                                        Toast.makeText(MainActivity.this, btn.getText().toString(), Toast.LENGTH_SHORT).show();
                                    }
                                });
                                //historyList.add(new History(history.getString("0")));
                            }

                            //HistoryAdapter adapter = new HistoryAdapter(MainActivity.this, historyList);
                            //recyclerView.setAdapter(adapter);
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        NetworkResponse networkResponse = error.networkResponse;
                        if (networkResponse != null && networkResponse.data != null) {
                            String jsonError = new String(networkResponse.data);
                            Toast.makeText(getApplicationContext(), "Error: " + jsonError, Toast.LENGTH_LONG).show();
                        }
                    }
                }
        );
        Volley.newRequestQueue(this).add(stringRequest);
    }

    private void loadTable() {
            StringRequest stringRequest = new StringRequest(Request.Method.POST, URL_MEASURES,
                    new Response.Listener<String>() {
                        @Override
                        public void onResponse(String response) {
                            try {

                                JSONArray array = new JSONArray(response);

                                for (int i = 0; i < array.length(); i++) {
                                        JSONObject measure = array.getJSONObject(i);

                                        measureList.add(new Measure(
                                                measure.getInt("id"),
                                                measure.getDouble("start_time"),
                                                measure.getDouble("power")
                                        ));

                                }

                                MeasureAdapter adapter = new MeasureAdapter(MainActivity.this, measureList);
                                recyclerView.setAdapter(adapter);
                            } catch (JSONException e) {
                                e.printStackTrace();
                            }
                        }
                    },
                    new Response.ErrorListener() {
                        @Override
                        public void onErrorResponse(VolleyError error) {
                            NetworkResponse networkResponse = error.networkResponse;
                            if (networkResponse != null && networkResponse.data != null) {
                                String jsonError = new String(networkResponse.data);
                                Toast.makeText(getApplicationContext(), "Error: " + jsonError, Toast.LENGTH_LONG).show();
                            }
                        }
                    }){
                @Override
                protected Map<String, String> getParams() throws AuthFailureError {
                    Map<String, String> params = new HashMap<String, String>();
                    params.put("table", "measurements_5_0");

                    return params;
                }
            };
            Volley.newRequestQueue(this).add(stringRequest);

    }

}