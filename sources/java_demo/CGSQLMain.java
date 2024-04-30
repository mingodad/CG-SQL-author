/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

import com.acme.cgsql.CQLDb;
import java.nio.charset.StandardCharsets;
import sample.*;

public class CGSQLMain {
  public static void main(String[] args) {
    // make an empty database (this can be replaced)
    CQLDb.open();

    // get result set handle
    long db = CQLDb.get();

    // get call result code and rowset
    var results = sample.SampleJNI.JavaDemo(db);

    System.out.println("Result code is: " +  results.get_result_code());

    var data = results.get_result_set();

    // use the results
    dumpResults(data);

    // release the connection
    CQLDb.close();
  }

  public static void dumpResults(SampleJNI.JavaDemoViewModel data) {
    System.out.println("Dumping Results");
    int count = data.getCount();
    System.out.println(String.format("count = %d", count));

    for (int i = 0; i < count; i++) {
      byte[] bytes = data.get_bytes(i);
      String s = new String(bytes, StandardCharsets.UTF_8);
      System.out.println(
          String.format(
              "Row %d: name:%s blob:%s age:%-7d(%s) thing:%f key1:%s key2:%s(%s)",
              i,
              data.get_name(i),
              s,
              data.get_age(i),
              data.get_age_IsEncoded() ? "encoded" : "clear",
              data.get_thing(i),
              data.get_key1(i),
              data.get_key2(i),
              data.get_key2_IsEncoded() ? "encoded" : "clear"
         )
     );

      // this could be done automatically in the helper, it just isn't yet
      // var child = data.get_my_child_result(i) should do the job
      var child = new SampleJNI.ChildViewModel(data.get_my_child_result(i));
      for (int j = 0; j < child.getCount(); j++) {
        System.out.println(
            String.format("--> Child Row %d: x:%d y:%s", j, child.get_x(j), child.get_y(j)));
      }
    }
  }
}
