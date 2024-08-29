<?php
header("Cache-Control: no-store, no-cache, must-revalidate, max-age=0");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");
?>
<html>

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta http-equiv="refresh" content="300">
    <style>
        table {
            border-collapse: collapse;
            max-width: 100%;
        }

        th,
        td {
            border: 1px solid #eee;
            padding: 0.5em;
            text-align: left;
        }

        th {
            background: #333;
            color: #ffffff;
            font-weight: 600;
        }
    </style>
</head>

<body>
    <h2>Combien il fait à la maison</h2>
    <!-- Votre 1er iframe (choisir "embed with script") -->

    <h2>L’humidité à la maison</h2>
    <!-- Votre 2e iframe (choisir "embed with script") -->

    <h2>Données</h2>
    <table>
        <tr>
            <th>Date</th>
            <th>Heure</th>
            <th>Temp.</th>
            <th>Humidité</th>
            <th>Volt</th>
        </tr>
        <?php
        $csvfiles = scandir("output");
        $min_lines = 100;
        $lines = array();
        if (($handle = fopen("output/" . $csvfiles[count($csvfiles) - 1], "r")) !== FALSE) {
            while (($data = fgetcsv($handle, 1000, ",")) !== FALSE) {
                $lines[] = "<tr>\n\t<td>" . $data[0] . '</td><td class="datetime">' .
                    $data[1] . '</td><td class="temp">' . $data[2] . '°C</td><td class="humidity">' .
                    $data[3] . '%</td><td class="battery">' . $data[4] . "</td>\n</tr>\n";
                $counter++;
            }
            fclose($handle);
        }
        echo implode(PHP_EOL, array_reverse($lines));
        ?>
    </table>

    <script type="text/javascript">
        datetimeElements = document.querySelectorAll(".datetime");
        for (element of datetimeElements) {
            let formattedTime = new Date(element.innerText).toLocaleTimeString();
            element.innerText = formattedTime;
        }
    </script>
</body>

</html>