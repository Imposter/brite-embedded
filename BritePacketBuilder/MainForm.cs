using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Ports;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace BritePacketBuilder
{
    public partial class MainForm : Form
    {
        private const int ReadTimeout = 1000;

        private string currentPath;
        private string openFile;
        private SerialPort port;
        private bool open;
        private Thread readThread;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            cmbPorts.Items.AddRange(SerialPort.GetPortNames());
        }

        private void btnOpenFolder_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog dialog = new FolderBrowserDialog
            {
                RootFolder = Environment.SpecialFolder.MyComputer,
                Description = "Browse Folder"
            };
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                currentPath = dialog.SelectedPath;
                loadFilesInDirectory();
            }
        }

        private void btnNew_Click(object sender, EventArgs e)
        {
            openFile = null;
            txtPacket.Clear();
            txtRemoteData.Clear();
            Text = "Brite Packet Builder - Untitled.brite *";
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(openFile))
            {
                btnSaveAs.PerformClick();
                return;
            }

            // Save file
            Text = "Brite Packet Builder - " + openFile;
            File.WriteAllText(Path.Combine(currentPath, openFile), txtPacket.Text);
        }

        private void btnSaveAs_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(currentPath))
            {
                txtLog.AppendText("Folder not open!\r\n");
                return;
            }

            SaveFileDialog dialog = new SaveFileDialog
            {
                Filter = "Brite Files (*.brite)|*.brite",
                InitialDirectory = currentPath
            };
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                openFile = Path.GetFileName(dialog.FileName);
                Text = "Brite Packet Builder - " + openFile;
                File.WriteAllText(Path.Combine(currentPath, openFile), txtPacket.Text);
                loadFilesInDirectory();
            }
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (port != null && open)
            {
                btnConnect.Text = "Connect";
                lblStatus.Text = "Disconnected";
                txtLog.AppendText("Disconnected from " + port.PortName + "\r\n");

                port.Close();
                open = false;
                if (readThread != null)
                {
                    readThread.Join();
                    readThread = null;
                }
                return;
            }

            string portName = (string)cmbPorts.SelectedItem;
            if (portName == null)
            {
                txtLog.AppendText("Select a valid COM port!\r\n");
                return;
            }

            string baudRate = (string)cmbBaudRates.SelectedItem;
            if (baudRate == null)
            {
                txtLog.AppendText("Select a valid baud rate!\r\n");
                return;
            }

            try
            {
                port = new SerialPort(portName, int.Parse(baudRate));
                port.DtrEnable = false;
                port.Open();
                port.BaseStream.ReadTimeout = 1000;
                open = true;

                txtLog.AppendText("Connected to " + portName + " using baud rate " + baudRate + "\r\n");
                lblStatus.Text = "Connected";
                btnConnect.Text = "Disconnect";
            }
            catch (Exception ex)
            {
                txtLog.AppendText("ERROR: " + ex + "\r\n");
            }
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (port != null && open && readThread == null)
            {
                // Parse data
                string[] splitData = txtPacket.Text.Split(' ');
                if (splitData.Length == 0)
                    return;

                // Clear previous response
                txtRemoteData.Clear();

                // Send data
                List<byte> buffer = new List<byte>();
                foreach (string str in splitData)
                {
                    if (string.IsNullOrWhiteSpace(str))
                        continue;

                    byte b = 0;
                    if (!byte.TryParse(str, out b))
                    {
                        txtLog.AppendText("\"" + str + "\" is not a proper byte value\r\n");
                        return;
                    }

                    buffer.Add(b);
                }

                txtLog.AppendText("Sent " + buffer.Count + " byte(s) to port\r\n");

                try
                {
                    port.BaseStream.Write(buffer.ToArray(), 0, buffer.Count);
                    port.BaseStream.Flush();

                    readThread = new Thread(() =>
                    {
                        if (txtRemoteData.InvokeRequired)
                        {
                            txtRemoteData.Invoke(new Action(() =>
                            {
                                txtRemoteData.Clear();

                                int bt = 0;
                                List<byte> bytes = new List<byte>();
                                try
                                {
                                    while ((bt = port.BaseStream.ReadByte()) != -1)
                                        bytes.Add((byte)bt);
                                }
                                catch (Exception)
                                {
                                }

                                if (bytes.Count > 0)
                                {
                                    txtRemoteData.Text = "Bytes: ";
                                    foreach (byte b in bytes)
                                        txtRemoteData.AppendText(b + " ");

                                    txtRemoteData.AppendText("\r\n\r\nString: ");

                                    foreach (byte b in bytes)
                                        txtRemoteData.AppendText(Encoding.ASCII.GetString(new[] { b }));
                                }
                                else
                                {
                                    txtRemoteData.AppendText("No Data Received");
                                }

                                readThread = null;
                            }));
                        }
                    });
                    readThread.Start();
                }
                catch (Exception ex)
                {
                    txtLog.AppendText("ERROR: " + ex + "\r\n");
                }
            }
        }

        private void tvFiles_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            string file = e.Node.Text;
            if (!string.IsNullOrWhiteSpace(file))
            {
                string path = Path.Combine(currentPath, file);
                if (File.Exists(path) && Path.GetExtension(path) == ".brite")
                {
                    openFile = file;
                    txtPacket.Text = File.ReadAllText(path);
                    Text = "Brite Packet Builder - " + openFile;
                }
            }
        }

        private void cmbPorts_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }

        private void cmbBaudRates_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = true;
        }

        private void txtPacket_KeyPress(object sender, KeyPressEventArgs e)
        {
            Text = "Brite Packet Builder - " + openFile + " *";
        }

        private void loadFilesInDirectory()
        {
            tvFiles.Nodes.Clear();

            var stack = new Stack<TreeNode>();
            var rootDirectory = new DirectoryInfo(currentPath);
            var node = new TreeNode(rootDirectory.Name) { Tag = rootDirectory };
            stack.Push(node);

            while (stack.Count > 0)
            {
                var currentNode = stack.Pop();
                var directoryInfo = (DirectoryInfo)currentNode.Tag;
                foreach (var directory in directoryInfo.GetDirectories())
                {
                    var childDirectoryNode = new TreeNode(directory.Name) { Tag = directory };
                    currentNode.Nodes.Add(childDirectoryNode);
                    stack.Push(childDirectoryNode);
                }

                foreach (var file in directoryInfo.GetFiles())
                    if (file.Extension == ".brite")
                        currentNode.Nodes.Add(new TreeNode(file.Name));
            }

            tvFiles.Nodes.Add(node);
            tvFiles.ExpandAll();
        }
    }
}
