package com.Training;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

public class ParsingJavaFiles {
	static int w = 0;
	static List<String> javaKeyWordsArray = new ArrayList<String>();

	public static void main(String[] args) throws IOException {
		FileWriter fw = null;
		File outputfile = new File("Processed.txt");
		BufferedWriter bw = null;
		HashMap<String, String> listOfKeywords = new HashMap<String, String>();
		listOfKeywords.put("abstract", "a");
		listOfKeywords.put("class", "b");
		listOfKeywords.put("public", "c");
		listOfKeywords.put("static", "d");
		listOfKeywords.put("void", "e");
		listOfKeywords.put("main", "f");
		listOfKeywords.put("for", "g");
		listOfKeywords.put("while", "h");
		listOfKeywords.put("if", "i");
		listOfKeywords.put("else", "j");
		listOfKeywords.put("switch", "k");
		listOfKeywords.put("case", "l");
		listOfKeywords.put("return", "m");
		listOfKeywords.put("break", "n");
		listOfKeywords.put("continue", "o");
		listOfKeywords.put("int", "p");
		listOfKeywords.put("float", "q");
		listOfKeywords.put("double", "r");
		listOfKeywords.put("long", "s");
		listOfKeywords.put("String", "t");
		listOfKeywords.put("char", "u");
		listOfKeywords.put("Integer", "v");
		listOfKeywords.put("Double", "w");
		listOfKeywords.put("System.out.println", "x");
		listOfKeywords.put("import", "y");
		listOfKeywords.put("args", "z");
		System.out.println(listOfKeywords.size());

		String packageclassPath = "D:\\Courses and Note taking\\Information Security\\ISProject\\JavaFiles";
		File folder = new File(packageclassPath);
		File[] listOfFiles = folder.listFiles();
		String fileStr;
		ArrayList<String> filePaths = new ArrayList<String>(); // An arraylist
		// of type
		// string that
		// shall save
		// the paths of
		// all the
		// files.
		for (File file : listOfFiles) {
			if (file.isFile()) {
				fileStr = file.getName();
				if (fileStr.endsWith(".JAVA") || fileStr.endsWith(".java")) // filtering
					// all
					// the
					// files
					// with
					// only
					// .java
					// extensions
					filePaths.add(fileStr);
			}
		}
		// System.out.println("ist is"+filePaths.get(2));
		for (String filePath : filePaths) {
			try {
				File fileinput = new File(packageclassPath + "\\" + filePath);
				BufferedReader br = new BufferedReader(
						new FileReader(fileinput));
				String line;
				while ((line = br.readLine()) != null) {
					String output[] = line.split("\\s+");
					// System.out.println("The length of the File is "+output.length);
					for (int i = 0; i < output.length; i++) {
						System.out.println(output[i] + " ");
						javaKeyWordsArray.add(output[i]);

					}

				}
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		System.out.println("keywords.length" + javaKeyWordsArray.size());
		System.out.println("keywordsarray =============================");
		String toWrite = "";
		System.out.println("jva" + javaKeyWordsArray.size());
		if (outputfile.exists()) {
			outputfile.delete();
		}
		if (!outputfile.exists()) {
			outputfile.createNewFile();
		}
		fw = new FileWriter("Processed.txt");
		
		bw = new BufferedWriter(fw);
		for (int i = 0; i < javaKeyWordsArray.size(); i++) {
			System.out.println(javaKeyWordsArray.get(i));
			if (listOfKeywords.containsKey(javaKeyWordsArray.get(i))) {
				
				toWrite = listOfKeywords.get(javaKeyWordsArray.get(i));
				System.out.println("printing tooooooooooowrite" + toWrite);
				
				bw.write(toWrite+" ");
				
		       
		       

			}

		}
		
		bw.close();
		fw.close();

	}
}
