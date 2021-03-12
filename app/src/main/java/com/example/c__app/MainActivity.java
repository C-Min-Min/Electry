package com.example.c__app;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
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

    private static final String URL_MEASURES = "http://192.168.88.241/php_api/GET.php";

    List<Measure> measureList;

    RecyclerView recyclerView;
    Button b;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        recyclerView = findViewById(R.id.recylcerView);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.setHasFixedSize(true);
        measureList = new ArrayList<>();

        b = (Button)findViewById(R.id.button);
        b.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                StringRequest stringRequest = new StringRequest(Request.Method.POST, URL_MEASURES,
                        new Response.Listener<String>() {
                            @Override
                            public void onResponse(String response) {
                                Toast.makeText(MainActivity.this, response.trim(), Toast.LENGTH_SHORT).show();
                            }
                        },
                        new Response.ErrorListener() {
                            @Override
                            public void onErrorResponse(VolleyError error) {
                                StringWriter sw = new StringWriter();
                                PrintWriter pw = new PrintWriter(sw);
                                error.printStackTrace(pw);
                            }
                        }){
                    @Override
                    protected Map<String, String> getParams() throws AuthFailureError {
                        Map<String, String> params = new HashMap<String, String>();
                        params.put("table", "measurements_5_0");

                        return params;
                    }
                };
                RequestQueue requestQueue = Volley.newRequestQueue(MainActivity.this);
                requestQueue.add(stringRequest);
            }
        });

    }

    private void loadMeasures() {

        StringRequest stringRequest = new StringRequest(Request.Method.GET, URL_MEASURES,
                new Response.Listener<String>() {
                    @Override
                    public void onResponse(String response) {

                        try {

                            JSONArray array = new JSONArray(response);

                            for (int i = 0; i < array.length(); i++) {
                                JSONArray childJsonArray = array.getJSONArray(i);
                                for (int j = 0; j < childJsonArray.length(); j++) {
                                    JSONObject measure = childJsonArray.getJSONObject(j);

                                    measureList.add(new Measure(
                                            measure.getInt("id"),
                                            measure.getDouble("start_time"),
                                            measure.getDouble("power")
                                    ));
                                }
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
                });
        Volley.newRequestQueue(this).add(stringRequest);
    }

}