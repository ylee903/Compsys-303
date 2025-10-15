<#macro simulation_imports position>
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import org.json.*;
</#macro>

<#macro simulation_in position>
receiveVariables();
</#macro>

<#macro simulation_out position>
sendVariables();
</#macro>

<#macro simulation_body position>
    public static BufferedReader stdInReader = new BufferedReader(new InputStreamReader(System.in));
            
    private static void receiveVariables() {
        try {
            String line = stdInReader.readLine();
            JSONObject json = new JSONObject(line);
            
            // Receive A
            if (json.has("A")) {
                ${tickdata_name}.A = json.getBoolean("A");
            }
            // Receive B
            if (json.has("B")) {
                ${tickdata_name}.B = json.getBoolean("B");
            }
            // Receive R
            if (json.has("R")) {
                ${tickdata_name}.R = json.getBoolean("R");
            }
            // Receive O
            if (json.has("O")) {
                ${tickdata_name}.O = json.getBoolean("O");
            }
            // Receive #ticktime
            if (json.has("#ticktime")) {
                _ticktime = json.getInt("#ticktime");
            }
            // Receive _GO
            if (json.has("_GO")) {
                ${tickdata_name}._GO = json.getBoolean("_GO");
            }
            // Receive _TERM
            if (json.has("_TERM")) {
                ${tickdata_name}._TERM = json.getBoolean("_TERM");
            }
            // Receive _cg11
            if (json.has("_cg11")) {
                ${tickdata_name}._cg11 = json.getBoolean("_cg11");
            }
            // Receive _cg13
            if (json.has("_cg13")) {
                ${tickdata_name}._cg13 = json.getBoolean("_cg13");
            }
            // Receive _cg15
            if (json.has("_cg15")) {
                ${tickdata_name}._cg15 = json.getBoolean("_cg15");
            }
            // Receive _cg20
            if (json.has("_cg20")) {
                ${tickdata_name}._cg20 = json.getBoolean("_cg20");
            }
            // Receive _cg5
            if (json.has("_cg5")) {
                ${tickdata_name}._cg5 = json.getBoolean("_cg5");
            }
            // Receive _cg7
            if (json.has("_cg7")) {
                ${tickdata_name}._cg7 = json.getBoolean("_cg7");
            }
            // Receive _g11
            if (json.has("_g11")) {
                ${tickdata_name}._g11 = json.getBoolean("_g11");
            }
            // Receive _g12
            if (json.has("_g12")) {
                ${tickdata_name}._g12 = json.getBoolean("_g12");
            }
            // Receive _g12_e2
            if (json.has("_g12_e2")) {
                ${tickdata_name}._g12_e2 = json.getBoolean("_g12_e2");
            }
            // Receive _g13
            if (json.has("_g13")) {
                ${tickdata_name}._g13 = json.getBoolean("_g13");
            }
            // Receive _g14
            if (json.has("_g14")) {
                ${tickdata_name}._g14 = json.getBoolean("_g14");
            }
            // Receive _g15
            if (json.has("_g15")) {
                ${tickdata_name}._g15 = json.getBoolean("_g15");
            }
            // Receive _g16
            if (json.has("_g16")) {
                ${tickdata_name}._g16 = json.getBoolean("_g16");
            }
            // Receive _g16_e1
            if (json.has("_g16_e1")) {
                ${tickdata_name}._g16_e1 = json.getBoolean("_g16_e1");
            }
            // Receive _g17
            if (json.has("_g17")) {
                ${tickdata_name}._g17 = json.getBoolean("_g17");
            }
            // Receive _g21
            if (json.has("_g21")) {
                ${tickdata_name}._g21 = json.getBoolean("_g21");
            }
            // Receive _g21_e2
            if (json.has("_g21_e2")) {
                ${tickdata_name}._g21_e2 = json.getBoolean("_g21_e2");
            }
            // Receive _g5
            if (json.has("_g5")) {
                ${tickdata_name}._g5 = json.getBoolean("_g5");
            }
            // Receive _g6
            if (json.has("_g6")) {
                ${tickdata_name}._g6 = json.getBoolean("_g6");
            }
            // Receive _g6_e1
            if (json.has("_g6_e1")) {
                ${tickdata_name}._g6_e1 = json.getBoolean("_g6_e1");
            }
            // Receive _g7
            if (json.has("_g7")) {
                ${tickdata_name}._g7 = json.getBoolean("_g7");
            }
            // Receive _g8
            if (json.has("_g8")) {
                ${tickdata_name}._g8 = json.getBoolean("_g8");
            }
            // Receive _pg16
            if (json.has("_pg16")) {
                ${tickdata_name}._pg16 = json.getBoolean("_pg16");
            }
            // Receive _pg21
            if (json.has("_pg21")) {
                ${tickdata_name}._pg21 = json.getBoolean("_pg21");
            }
            // Receive _pg21_e2
            if (json.has("_pg21_e2")) {
                ${tickdata_name}._pg21_e2 = json.getBoolean("_pg21_e2");
            }
            // Receive _taken_transitions
            if (json.has("_taken_transitions")) {
                JSONArray _array = json.getJSONArray("_taken_transitions");
                for (int i = 0; i < _array.length(); i++) {
                    ${tickdata_name}._taken_transitions[i] = _array.getInt(i);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
           // Ignore other input
        }
    }
    
    private static void sendVariables() {
        JSONObject json = new JSONObject();
        
        // Send A
        json.put("A", JSONObject.wrap(${tickdata_name}.A));
        // Send B
        json.put("B", JSONObject.wrap(${tickdata_name}.B));
        // Send R
        json.put("R", JSONObject.wrap(${tickdata_name}.R));
        // Send O
        json.put("O", JSONObject.wrap(${tickdata_name}.O));
        // Send #ticktime
        json.put("#ticktime", _ticktime);
        // Send _GO
        json.put("_GO", JSONObject.wrap(${tickdata_name}._GO));
        // Send _TERM
        json.put("_TERM", JSONObject.wrap(${tickdata_name}._TERM));
        // Send _cg11
        json.put("_cg11", JSONObject.wrap(${tickdata_name}._cg11));
        // Send _cg13
        json.put("_cg13", JSONObject.wrap(${tickdata_name}._cg13));
        // Send _cg15
        json.put("_cg15", JSONObject.wrap(${tickdata_name}._cg15));
        // Send _cg20
        json.put("_cg20", JSONObject.wrap(${tickdata_name}._cg20));
        // Send _cg5
        json.put("_cg5", JSONObject.wrap(${tickdata_name}._cg5));
        // Send _cg7
        json.put("_cg7", JSONObject.wrap(${tickdata_name}._cg7));
        // Send _g11
        json.put("_g11", JSONObject.wrap(${tickdata_name}._g11));
        // Send _g12
        json.put("_g12", JSONObject.wrap(${tickdata_name}._g12));
        // Send _g12_e2
        json.put("_g12_e2", JSONObject.wrap(${tickdata_name}._g12_e2));
        // Send _g13
        json.put("_g13", JSONObject.wrap(${tickdata_name}._g13));
        // Send _g14
        json.put("_g14", JSONObject.wrap(${tickdata_name}._g14));
        // Send _g15
        json.put("_g15", JSONObject.wrap(${tickdata_name}._g15));
        // Send _g16
        json.put("_g16", JSONObject.wrap(${tickdata_name}._g16));
        // Send _g16_e1
        json.put("_g16_e1", JSONObject.wrap(${tickdata_name}._g16_e1));
        // Send _g17
        json.put("_g17", JSONObject.wrap(${tickdata_name}._g17));
        // Send _g21
        json.put("_g21", JSONObject.wrap(${tickdata_name}._g21));
        // Send _g21_e2
        json.put("_g21_e2", JSONObject.wrap(${tickdata_name}._g21_e2));
        // Send _g5
        json.put("_g5", JSONObject.wrap(${tickdata_name}._g5));
        // Send _g6
        json.put("_g6", JSONObject.wrap(${tickdata_name}._g6));
        // Send _g6_e1
        json.put("_g6_e1", JSONObject.wrap(${tickdata_name}._g6_e1));
        // Send _g7
        json.put("_g7", JSONObject.wrap(${tickdata_name}._g7));
        // Send _g8
        json.put("_g8", JSONObject.wrap(${tickdata_name}._g8));
        // Send _pg16
        json.put("_pg16", JSONObject.wrap(${tickdata_name}._pg16));
        // Send _pg21
        json.put("_pg21", JSONObject.wrap(${tickdata_name}._pg21));
        // Send _pg21_e2
        json.put("_pg21_e2", JSONObject.wrap(${tickdata_name}._pg21_e2));
        // Send _taken_transitions
        json.put("_taken_transitions", JSONObject.wrap(${tickdata_name}._taken_transitions));
        
        System.out.println(json.toString());
    }
</#macro>
