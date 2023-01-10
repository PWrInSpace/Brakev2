import pandas as pd

class FlightData:
    
    DATA_TO_COPY = ["ax", "ay", "az", "gx", "gy", "gz", "pressure", "altitude"]

    def __init__(self):
        self.prepared_data_file_name = "prep_brejk_data.txt"
        pass

    def _get_data_interval(self, df):
        prepared_df = pd.DataFrame()
        next_data_interval = []
        previous_time = 0
        for index, row in df.iterrows():
            time = row["uptime"]
            if index == 0:
                next_data_interval.append(str(float(time) - float(time)))
            else:
                next_data_interval.append(str(float(time) - float(previous_time)))
            previous_time = time

        prepared_df["data_interval"] = next_data_interval
        return prepared_df

    def _preapare_data(self, df):
        data_strings = []
        for index, row in df.iterrows():
            data_row = ""
            for name in self.DATA_TO_COPY:
                data_row += str(row[name]) + ";"

            data_strings.append(data_row[:-1])

        return data_strings

    def qtcb_load_data_from_file(self, **kwargs) -> bool:
        if "path" not in kwargs:
            raise RuntimeError("Flight data reader: Can not find path in arguments")

        df = pd.read_csv(kwargs["path"], delimiter=";")
        prepared_data = self._preapare_data(df)

        with open(self.prepared_data_file_name, "w") as file:
            for line in prepared_data:
                file.write(line + '\n')

        return True

if __name__ == "__main__":
   fd = FlightData()
   fd.load_data_from_file(path="data.txt")