import pandas as pd

class FlightData:
    DATA_TO_COPY = ["ax", "ay", "az", "gx", "gy", "gz", "pressure", "tempreature"]

    def __init__(self):
        self.prepared_df = None
        self.prepared_df_pos = 0

    # def _get_data_interval(self, df):
    #     prepared_df = pd.DataFrame()
    #     next_data_interval = []
    #     previous_time = 0
    #     for index, row in df.iterrows():
    #         time = row["uptime"]
    #         if index == 0:
    #             next_data_interval.append(str(float(time) - float(time)))
    #         else:
    #             next_data_interval.append(str(float(time) - float(previous_time)))
    #         previous_time = time

    #     prepared_df["data_interval"] = next_data_interval
    #     return prepared_df

    def getPreparedLine(self) -> str | None:
        if self.prepared_df is None:
            return None
        if self.prepared_df_pos >= len(self.prepared_df):
            return None

        line = self.prepared_df.iloc[[self.prepared_df_pos]]
        self.prepared_df_pos += 1
        return line.to_string(index = False, header=False)


    def getPreparedDataLength(self):
        if self.prepared_df is None:
            return 0
        return len(self.prepared_df)

    def getPreapredDataPosition(self):
        return self.prepared_df_pos

    def _preapare_data(self, df):
        data_strings = []
        for index, row in df.iterrows():
            data_row = ""
            for name in self.DATA_TO_COPY:
                data_row += str(row[name]) + ";"

            data_strings.append(data_row[:-1])

        return data_strings

    def load_data(self, path) -> bool:
        df = pd.read_csv(path, delimiter=";")
        prepared_data = self._preapare_data(df)
        self.prepared_df = pd.DataFrame(prepared_data)
        # print(self.prepared_df.iloc[[0]])
        return True


if __name__ == "__main__":
   fd = FlightData()
   fd.load_data(path="data.txt")
   print(fd.getPreparedLine())